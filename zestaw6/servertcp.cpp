#include <stdarg.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <map>
// Te dwa pliki nagłówkowe są specyficzne dla Linuksa z biblioteką glibc:
#include <sys/epoll.h>
#include <sys/syscall.h>

#define DATA_SIZE 4096

std::map<long int, char*> client_message;   //  Mapa, która zbiera zapytania klientów

// Standardowa procedura tworząca nasłuchujące gniazdko TCP.
int listening_socket_tcp_ipv4(in_port_t port)
{
    int s;

    if ((s = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("socket");
        return -1;
    }

    struct sockaddr_in a;
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = htonl(INADDR_ANY);
    a.sin_port = htons(port);

    if (bind(s, (struct sockaddr *) &a, sizeof(a)) == -1) {
        perror("bind");
        close(s);
        return -1;
    }

    if (listen(s, 10) == -1) {
        perror("listen");
        close(s);
        return -1;
    }

    return s;
}


// Pomocnicze funkcje do drukowania na ekranie komunikatów uzupełnianych
// o znacznik czasu oraz identyfikatory procesu/wątku. Będą używane do
// raportowania przebiegu pozostałych operacji we-wy.
void log_printf(const char * fmt, ...)
{
    // bufor na przyrostowo budowany komunikat, len mówi ile już znaków ma
    char buf[1024];
    int len = 0;

    struct timespec date_unix;
    struct tm date_human;
    long int usec;
    if (clock_gettime(CLOCK_REALTIME, &date_unix) == -1) {
        perror("clock_gettime");
        return;
    }
    if (localtime_r(&date_unix.tv_sec, &date_human) == NULL) {
        perror("localtime_r");
        return;
    }
    usec = date_unix.tv_nsec / 1000;

    // getpid() i gettid() zawsze wykonują się bezbłędnie
    pid_t pid = getpid();
    pid_t tid = syscall(SYS_gettid);

    len = snprintf(buf, sizeof(buf), "%02i:%02i:%02i.%06li PID=%ji TID=%ji ",
                date_human.tm_hour, date_human.tm_min, date_human.tm_sec,
                usec, (intmax_t) pid, (intmax_t) tid);
    if (len < 0) {
        return;
    }

    va_list ap;
    va_start(ap, fmt);
    int i = vsnprintf(buf + len, sizeof(buf) - len, fmt, ap);
    va_end(ap);
    if (i < 0) {
        return;
    }
    len += i;

    // zamień \0 kończące łańcuch na \n i wyślij całość na stdout, czyli na
    // deskryptor nr 1; bez obsługi błędów aby nie komplikować przykładu
    buf[len] = '\n';
    write(1, buf, len + 1);
}

void log_perror(const char * msg)
{
    log_printf("%s: %s", msg, strerror(errno));
}

void log_error(const char * msg, int errnum)
{
    log_printf("%s: %s", msg, strerror(errnum));
}

// Pomocnicze funkcje wykonujące pojedynczą operację we-wy oraz wypisujące
// szczegółowe komunikaty o jej działaniu.

int accept_verbose(int srv_sock)
{
    struct sockaddr_in a;
    socklen_t a_len = sizeof(a);

    log_printf("calling accept() on descriptor %i", srv_sock);
    int rv = accept(srv_sock, (struct sockaddr *) &a, &a_len);
    if (rv == -1) {
        log_perror("accept");
    } else {
        char buf[INET_ADDRSTRLEN];
        if (inet_ntop(AF_INET, &a.sin_addr, buf, sizeof(buf)) == NULL) {
            log_perror("inet_ntop");
            strcpy(buf, "???");
        }
        log_printf("new client %s:%u, new descriptor %i",
                        buf, (unsigned int) ntohs(a.sin_port), rv);
    }
    return rv;
}

ssize_t read_verbose(int fd, void * buf, size_t nbytes)
{
    log_printf("calling read() on descriptor %i", fd);
    ssize_t rv = read(fd, buf, nbytes);
    if (rv == -1) {
        log_perror("read");
    } else {
        log_printf("received %zi bytes on descriptor %i", rv, fd);
    }
    return rv;
}

ssize_t write_verbose(int fd, void * buf, ssize_t nbytes)
{
    log_printf("calling write() on descriptor %i", fd);
    ssize_t rv = write(fd, buf, nbytes);
    if (rv == -1) {
        log_perror("write");
    } else if (rv < nbytes) {
        log_printf("partial write on %i, wrote only %zi of %zu bytes",
                        fd, rv, nbytes);
    } else {
        log_printf("wrote %zi bytes on descriptor %i", rv, fd);
    }
    return rv;
}

int close_verbose(int fd)
{
    log_printf("closing descriptor %i", fd);
    int rv = close(fd);
    if (rv == -1) {
        log_perror("close");
    }
    return rv;
}

// Procedury przetwarzające pojedynczą porcję danych przysłaną przez klienta.

char* operation(char* buf, ssize_t& bytes_read)
{
    char number[DATA_SIZE];
    char resp_tmp[DATA_SIZE] = "";
    char answer[DATA_SIZE] = "";
    char mode = '\0';  // Zmienna zapamiętująca znak działania
    long int sum = 0;   // zmienna będąca sumą podanego działania
    bool error = false; // boolean informujący czy wystąpił błąd
    bool znak_wyst = false; // boolean informujący czy pojawił się już znak '+' lub '-'

    while(1)
    {  
        for (int i = 0, size_of_number = 0; i < bytes_read; i++)    // size_of_number - zmienna informująca z ilu cyfr składa się liczba
        {    
            // Sprawdzenie czy aktualny znak w wiadomości jest spacją
            if(buf[i] == ' ')
            {
                error = true;
                continue;
            }
            
            //  Jeśli wystąpił błąd to pomijamy aktualną linię
            if(error == true && buf[i] != '\r')
            {
                continue;
            }

            // Sprawdzenie, czy aktualny znak w wiadomości jest znakiem działania lub końcem linii
            else if(buf[i] == '+' || buf[i] == '-' || (buf[i] == '\r' && buf[i + 1] == '\n'))
            {
                
                //  W przypadku wystąpienia błędu pomijamy resztę zapytanie do końca linii
                if(error && buf[i] != '\r')
                {
                    continue;
                }
                
                //  Sprawdzanie, czy klient nie wysłał pustej wiadomości
                if((buf[i] == '\r' && buf[i + 1] == '\n') && znak_wyst == false && size_of_number == 0)
                {
                    error = true;
                }

                // Sprawdzenie, czy klient nie wysłał wiadomości zaczynającej się od znaku (np. -2+3 lub +2+3)
                if(size_of_number == 0 && (buf[i] == '+' || buf[i] == '-'))
                {
                    error = true;
                }

                // Sprawdzenie, czy w wiadomości od klienta nie występują 2 znaki po sobie (np. 100++15 lub 100-+15)
                if((buf[i] == '+' && buf[i - 1] == '+') ||(buf[i] == '-' && buf[i - 1] == '-') || (buf[i] == '+' && buf[i - 1] == '-') || (buf[i] == '-' && buf[i - 1] == '+'))
                {
                    error = true;

                }

                if(i == bytes_read - 1 && (buf[i] == '+' || buf[i] == '-'))
                {
                    error = true;
                }
                
                number[size_of_number] = '\0';
                long int conv_number = strtoul(number, NULL, 10);

                // Sprawdzanie, czy liczba została poprawnie przekonwertowana   (sprawdzam czy liczba ascii jedności itd. - liczba ascii 0 jest równa modulo liczby test)
                long int test = conv_number;
                for (int k = size_of_number - 1; k >= 0; k--)
                {
                    if(number[k] - '0' != test % 10)
                    {
                        error = true;
                    }

                    test = test / 10;
                }

                // Sprawdzanie, czy wystąpiło przepełnienie
                if(sum > 0 && conv_number > 0 && sum + conv_number < 0)
                {
                    error = true;

                }
                if(sum < (sum - conv_number))
                {
                    error = true;
                }

                else if(buf[i] == '\r' && buf[i + 1] != '\n')
                {
                    error = true;
                }

                // Sprawdzenie, czy jest to pierwsze wystąpienie znaku działania
                if(znak_wyst == false)
                {
                    sum = sum + conv_number;
                    znak_wyst = true;
                }

                // Dodawanie
                if(mode == '+' && buf[i] != '\r')
                {
                    sum = sum + conv_number;
                }
                // Odejmowanie
                if(mode == '-' && buf[i] != '\r')
                {
                    sum = sum - conv_number;
                }

                // Sprawdzenie, czy i gdzie występuje znak końca linii
                if(buf[i] == '\r' && buf[i + 1] == '\n')
                {
                    
                    // Obsługa przypadków bez znaku końca linii na końcu datagramu
                    if(mode == '+')
                    {
                        sum = sum + conv_number;
                    }
                    
                    if(mode == '-')
                    {
                        sum = sum - conv_number;
                    }
                    
                    if(!error)
                    {
                        sprintf(answer, "%ld\r\n", sum);

                        strcat(resp_tmp, answer);

                    }
                    // Odpowiedź serwera w przypadku wystąpienia błędu
                    else
                    {
                        strcat(resp_tmp, "ERROR\r\n");

                    }

                    
                    strcpy(buf, resp_tmp);
                    bytes_read = i + 2;
                    return buf;

                }

                if(buf[i] == '+')
                {
                    mode = '+';
                }
                else if(buf[i] == '-')
                {
                    mode = '-';
                }
                size_of_number = 0; 

            }
            // Sprawdzenie, czy aktualny znak w wiadomości jest cyfrą
            else if(buf[i] >= '0' && buf[i] <= '9')
            {
                if(size_of_number == 0 && (buf[i] == '0' && buf[i + 1] != '+' && buf[i + 1] != '-' && buf[i + 1] != '\r'))
                {
                    continue;
                }
                number[size_of_number] = buf[i];
                size_of_number++;
                if(size_of_number > 21)
                {
                    error = true;
                    continue;
                }
            }
            // Przypadek dla innych znaków ASCII
            else
            {
                error = true;
                continue;
            }
        }
        abort();    
    }

}

bool check_rn(char* buf)
{
    for(long unsigned int i = 0; i < strlen(buf); i++)
    {
        if(buf[i] == '\r' && buf[i + 1] == '\n')
        {
            return true;
        }
    }
    return false;
}

ssize_t read_and_write(int sock)
{
    char buf[DATA_SIZE];

    ssize_t bytes_read = read_verbose(sock, buf, sizeof(buf));
    if (bytes_read < 0) {
        return -1;
    }

    buf[bytes_read] = '\0';
    strcat(client_message[sock], buf);

    if(bytes_read > 0)
    {
        while(check_rn(client_message[sock]))
        {
            strcpy(buf, client_message[sock]);
            bytes_read = strlen(buf); 
            char buf_tmp[DATA_SIZE] = "";
            char * data = operation(buf, bytes_read);
            size_t data_len = strlen(data);
            strncpy(buf_tmp, client_message[sock] + bytes_read, strlen(client_message[sock]) - bytes_read);
            strcpy(client_message[sock], buf_tmp);
            ssize_t bytes_written = write_verbose(sock, data, data_len);
            if (bytes_written < 0) {
                return -1;
            } 
        }   
    }

    return bytes_read;
}

int add_fd_to_epoll(int fd, int epoll_fd)
{
    char* buf_client = new char[DATA_SIZE]();
    client_message[fd] = buf_client;
    log_printf("adding descriptor %i to epoll instance %i", fd, epoll_fd);
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));
    ev.events = EPOLLIN;    // rodzaj interesujących nas zdarzeń
    ev.data.fd = fd;        // dodatkowe dane, jądro nie zwraca na nie uwagi
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &ev);
    if (rv == -1) {
        log_perror("epoll_ctl(ADD)");
    }
    return rv;
}

int remove_fd_from_epoll(int fd, int epoll_fd)
{
    delete[] client_message[fd];
    log_printf("removing descriptor %i from epoll instance %i", fd, epoll_fd);
    int rv = epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, NULL);
    if (rv == -1) {
        log_perror("epoll_ctl(DEL)");
    }
    return rv;
}

#define MAX_EVENTS 8

void epoll_loop(int srv_sock)
{
    // w dzisiejszych czasach wartość argumentu nie ma znaczenia
    int epoll_fd = epoll_create(10);
    if (epoll_fd == -1) {
        log_perror("epoll_create");
        return;
    }
    log_printf("epoll instance created, descriptor %i", epoll_fd);

    if (add_fd_to_epoll(srv_sock, epoll_fd) == -1) {
        return;
    }

    while (true) {
        log_printf("calling epoll()");
        struct epoll_event events[MAX_EVENTS];
        // timeout równy -1 oznacza czekanie w nieskończoność
        int num = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        if (num == -1) {
            log_perror("epoll");
            break;
        }
        // epoll wypełniła num początkowych elementów tablicy events
        log_printf("number of events = %i", num);

        for (int i = 0; i < num; ++i) {
            // deskryptor został wcześniej zapisany jako dodatkowe dane
            int fd = events[i].data.fd;
            // typ zgłoszonego zdarzenia powinien zawierać "gotów do odczytu"
            if ((events[i].events & EPOLLIN) == 0) {
                // niby nigdy nie powinno się zdarzyć, ale...
                log_printf("descriptor %i isn't ready to read", fd);
                continue;
            }

            if (fd == srv_sock) {

                int s = accept_verbose(srv_sock);
                if (s == -1) {
                    close_verbose(fd);
                    return;
                }
                if (add_fd_to_epoll(s, epoll_fd) == -1) {
                    close_verbose(fd);
                    return;
                }

            } else {    // fd != srv_sock

                if (read_and_write(fd) <= 0) {
                    // druga strona zamknęła połączenie lub wystąpił błąd
                    remove_fd_from_epoll(fd, epoll_fd);
                    close_verbose(fd);
                }

            }
        }
    }
}

int main(int argc, char * argv[])
{
    long int srv_port = 2020;
    int srv_sock;
    void (*main_loop)(int);

    // Przetwórz argumenty podane w linii komend, ustaw na ich podstawie
    // wartości zmiennych srv_port i main_loop.
    main_loop = epoll_loop;

    errno = 0;

    // Stwórz gniazdko i uruchom pętlę odbierającą przychodzące połączenia.
    if ((srv_sock = listening_socket_tcp_ipv4(srv_port)) == -1) {
        return 1;
    }

    log_printf("starting main loop");
    main_loop(srv_sock);
    log_printf("main loop done");

    if (close(srv_sock) == -1) {
        log_perror("close");
        return 1;
    }

    return 0;
}
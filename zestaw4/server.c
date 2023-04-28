#define _POSIX_C_SOURCE 200809L
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<sys/time.h>

#define PORT 2020
#define DATA_SIZE 65520

int main(void)
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(INADDR_ANY) },
        .sin_port = htons(PORT)
    };

    rc = bind(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("bind");
        return 1;
    }

    bool keep_on_handling_clients = true;
    while (keep_on_handling_clients) {

        struct sockaddr_in clnt_addr;
        socklen_t clnt_addr_len;
        clnt_addr_len = sizeof(clnt_addr);

        unsigned char buf[DATA_SIZE + 1];
        char number[DATA_SIZE];
        int size_of_number = 0; // zmienna informująca z ilu cyfr składa się liczba
        long int sum = 0;   // zmienna będąca sumą podanego działania
        bool error = false; // boolean informujący czy wystąpił błąd
        bool data = false;  // boolean informujący czy datagram kończy się \n lub \r\n

        cnt = recvfrom(sock, buf, sizeof(buf), 0,
                (struct sockaddr *) & clnt_addr, & clnt_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return 1;
        }

        //printf("received %zi bytes\n", cnt);
        bool znak_wyst = false; // boolean informujący czy pojawił się już znak '+' lub '-'
        for (int i = 0, j = 0, ind_znak = 0; i <= cnt; i++) //Nadmiarowy loop dla i równego cnt jest dla obsłużenia datagramów bez \n lub \r\n na końcu
        {            
            // Sprawdzenie czy aktualny znak w wiadomości jest spacją
            if(buf[i] == ' ')
            {
                error = true;
                break;
            }
            // Sprawdzenie, czy aktualny znak w wiadomości jest znakiem działania lub końcem linii
            else if(buf[i] == '+' || buf[i] == '-' || buf[i] == '\n' || (buf[i] == '\r' && buf[i + 1] == '\n') || i == cnt)
            {
                //  Sprawdzanie, czy klient nie wysłał pustej wiadomości
                if((buf[i] == '\n' || (buf[i] == '\r' && buf[i + 1] == '\n')) && znak_wyst == false && size_of_number == 0)
                {
                    error = true;
                    break;
                }

                // Sprawdzenie, czy klient nie wysłał wiadomości zaczynającej się od znaku (np. -2+3 lub +2+3)
                if(size_of_number == 0 && (buf[i] == '+' || buf[i] == '-'))
                {
                    error = true;
                    break;
                }

                // Sprawdzenie, czy w wiadomości od klienta nie występują 2 znaki po sobie (np. 100++15 lub 100-+15)
                if((buf[i] == '+' && buf[i - 1] == '+') ||(buf[i] == '-' && buf[i - 1] == '-') || (buf[i] == '+' && buf[i - 1] == '-') || (buf[i] == '-' && buf[i - 1] == '+'))
                {
                    error = true;
                    break;
                }

                if(i == cnt - 1 && (buf[i] == '+' || buf[i] == '-'))
                {
                    error = true;
                    break;
                }
                
                number[j] = '\0';

                long int conv_number = atoi(number);

                // Sprawdzanie, czy liczba została poprawnie przekonwertowana
                long int test = conv_number;
                for (int k = j - 1; k >= 0; k--)
                {
                    if(number[k] - '0' != test % 10)
                    {
                        error = true;
                        break;
                    }

                    test = test / 10;
                }

                // Sprawdzanie, czy wystąpiło przepełnienie
                if(sum > sum + conv_number || error == true)
                {
                    error = true;
                    break;
                }
                if(sum < sum - conv_number || error == true)
                {
                    error = true;
                    break;
                }

                // Sprawdzenie, czy jest to pierwsze wystąpienie znaku działania
                if(znak_wyst == false)
                {
                    sum = sum + conv_number;
                    znak_wyst = true;
                }

                // Obsługa przypadków bez znaku końca linii na końcu datagramu
                if(i == cnt && buf[ind_znak] == '+'){
                    sum = sum + conv_number;
                    break;
                }
                
                if(i == cnt && buf[ind_znak] == '-'){
                    sum = sum - conv_number;
                    break;
                }

                // Dodawanie
                if(buf[ind_znak] == '+')
                {
                    sum = sum + conv_number;

                }
                // Odejmowanie
                if(buf[ind_znak] == '-')
                {
                    sum = sum - conv_number;
                }
                
                ind_znak = i;   // Zmienna zapamiętująca poprzednie wystąpienie znaku działania
                j = 0; 

                // Sprawdzenie, czy i gdzie występuje znak końca linii
                if((buf[i] == '\r' && buf[i + 1] == '\n') && i == cnt - 2)
                {
                    data = true;
                    break;
                }
                else if((buf[i] == '\r' && buf[i + 1] == '\n') && i != cnt - 2)
                {
                    error = true;
                    break;
                }
                else if(buf[i] == '\n' && i == cnt - 1)
                {
                    data = true;
                    break;
                }
                else if(buf[i] == '\n' && i != cnt - 1)
                {
                    error = true;
                    break;
                }
            }
            // Sprawdzenie, czy aktualny znak w wiadomości jest cyfrą
            else if(buf[i] >= '0' && buf[i] <= '9')
            {
                number[j] = buf[i];
                size_of_number++;
                j++;
                if(j == 21)
                {
                    error = true;
                    break;
                }
            }
            // Przypadek dla innych znaków ASCII
            else
            {
                error = true;
                break;
            }
        }

        // Odpowiedź serwera w przypadku braku błędu
        if(!error)
        {
            int out = 0;
            if(data)
            {
                out = sprintf(number, "%ld\r\n", sum);
            }
            else
            {
                out = sprintf(number, "%ld", sum);
            }

            cnt = sendto(sock, number, out, 0,
                    (struct sockaddr *) & clnt_addr, clnt_addr_len);
            if (cnt == -1) {
                perror("sendto");
                return 1;
            }
            //printf("sent %zi bytes\n", cnt);
        }
        // Odpowiedź serwera w przypadku wystąpienia błędu
        else
        {
            cnt = sendto(sock, "ERROR", 5, 0,
                    (struct sockaddr *) & clnt_addr, clnt_addr_len);
            if (cnt == -1) {
                perror("sendto");
                return 1;
            }
        }
    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
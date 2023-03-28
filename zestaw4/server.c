/*
Napisz serwer UDP/IPv4 nasłuchujący na porcie nr 2020 i implementujący powyższy protokół.
    "Jak wyglądają wyrażenia? Czy są może w odwrotnej notacji polskiej?
        Wyrażenia są w notacji infiksowej, czyli codziennej. Poprawne wyrażenia to np. 2+2 lub 45+1044-512+0+28-0077.
        Nie będziemy pozwalać na używanie plusa w roli znaku liczby, może on występować tylko w roli operatora dodawania. Co za tym idzie, +2+2, 100++15 itp. są niepoprawnymi zapytaniami.
        Inne przykłady niepoprawnych wyrażeń: 2 + 2 (nie wolno używać spacji), -20+60 (rozważamy protokół dodawania i odejmowania liczb naturalnych, a -20 nie jest liczbą naturalną).

     Odejmowanie liczb naturalnych może dać wynik nie będący liczbą naturalną. Co wtedy?
        Można było albo zdecydować się na zwracanie błędu, albo dopuścić ujemne wyniki. Wybraliśmy tę drugą możliwość. 
        Do specyfikacji trzeba dopisać „wyniki zwracane przez serwer są liczbami całkowitymi”.

     Czy wyrażenie zawierające tylko jedną liczbę, bez żadnego operatora, jest dopuszczalne? A wyrażenie puste, czyli zapytanie długości zero bajtów?
        Wyrażenia z jedną liczbą będziemy traktować jako poprawne. Wyrażenia puste mają powodować zwrócenie przez serwer komunikatu o błędzie.

     Jak wielkie mogą być liczby? Czy jest może jakiś limit, np. nie więcej niż dziesięć cyfr?
        Nie będziemy narzucali ograniczeń na to, co może wysyłać klient, postawimy za to pewne minimalne wymagania serwerowi.
        Aby serwer mógł być uznany za poprawnie implementujący protokół, musi być on w stanie poprawnie obliczać wartości wyrażeń, w których wszystkie liczby, 
        wynik końcowy i wyniki pośrednie mieszczą się w zakresie typu int16_t. Serwer może używać do obliczeń typu o większym zakresie wartości.
        Co za tym idzie, po wysłaniu serwerowi 2+2 albo 10000+20000 na pewno dostaniemy w odpowiedzi poprawnie obliczoną sumę, ale gdy wyślemy 30000+30000 możemy dostać albo 60000, albo ERROR.

     Czy jest jakiś limit na długość zapytań-datagramów wysyłanych przez klienta?
        Nie, na poziomie naszego protokołu aplikacyjnego nie. Natomiast datagramowe protokoły transportowe, z których będzie on korzystał, będą miały jakieś swoje własne limity.
        W szczególności UDP ma limit ciut poniżej 64 KiB."

Serwer musi weryfikować odebrane dane i zwracać komunikat o błędzie jeśli są one nieprawidłowe w sensie zgodności ze specyfikacją protokołu.

W kodzie używaj zmiennych roboczych któregoś ze standardowych typów całkowitoliczbowych (int, long int, int32_t, itd.). 
Co za tym idzie, odebrany ciąg cyfr będzie mógł reprezentować liczbę zbyt dużą, aby dało się ją zapisać w zmiennej wybranego typu.
Może też się zdarzyć, że podczas dodawania bądź odejmowania wystąpi przepełnienie (ang. integer overflow / wraparound). 
Serwer ma obowiązek wykrywać takie sytuacje i zwracać błąd. Uwadze Państwa polecam pliki nagłówkowe limits.h oraz stdint.h,
w których znaleźć można m.in. parę stałych INT_MIN i INT_MAX oraz parę INT32_MIN i INT32_MAX.
*/

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
        bool data = false;  // boolean informujący czy przeczytaliśmy cały datagram

        cnt = recvfrom(sock, buf, sizeof(buf), 0,
                (struct sockaddr *) & clnt_addr, & clnt_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return 1;
        }

        printf("received %zi bytes\n", cnt);
        bool znak_wyst = false; // boolean informujący czy pojawił się już znak '+' lub '-'
        for (int i = 0, j = 0, ind_znak = 0; i <= cnt; i++)
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
                    data = true;
                    break;
                }

                // Sprawdzenie, czy klient nie wysłał wiadomości zaczynającej się od znaku (np. -2+3 lub +2+3)
                if(size_of_number == 0 && (buf[i] == '+' || buf[i] == '-'))
                {
                    error = true;
                    break;
                }

                // Sprawdzenie, czy w wiadomości od klienta nie występują 2 znaki po sobie (np. 100++15 lub 100-+15)
                if((buf[i] == '+' && buf[i - 1] == '+') ||(buf[i] == '-' && buf[i - 1] == '-'))
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
                    data = true;
                    break;
                }
                
                if(i == cnt && buf[ind_znak] == '-'){
                    sum = sum - conv_number;
                    data = true;
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

                // Sprawdzenie, czy występuje znak końca linii
                if(buf[i] == '\n' || (buf[i] == '\r' && buf[i + 1] == '\n'))
                {
                    data = true;
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
                out = sprintf(number, "%ld\n", sum);
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
            cnt = sendto(sock, "ERROR\n", 6, 0,
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
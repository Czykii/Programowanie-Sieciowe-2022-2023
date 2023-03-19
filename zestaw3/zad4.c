/*
Napisz serwer UDP/IPv4 nasłuchujący na porcie nr 2020 i implementujący powyższy protokół.
    ""


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
#define DATA_SIZE 65535

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

    /*struct timeval timeout = {
        timeout.tv_sec = 10,
        timeout.tv_usec = 0
    };

    if (getsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout)) == -1){
        perror("getsockopt rcv");
        return 1;
    }

    if(setsockopt(sock, SOL_SOCKET, SO_SNDTIMEO, &timeout, sizeof(timeout)) == -1){
        perror("getsockopt snd");
        return 1;
    }*/

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
        long int sum = 0;   // zmienna będąca sumą podanego działania
        bool error = false; // boolean informujący czy wystąpił błąd
        bool data = false;  // boolean informujący czy przeczytaliśmy cały datagram

        cnt = recvfrom(sock, buf, sizeof(buf), 0,
                (struct sockaddr *) & clnt_addr, & clnt_addr_len);
        if (cnt == -1) {
            perror("recvfrom");
            return 1;
        }

        for (int i = 0, j = 0; i < sizeof(buf); i++)
        {
            if(buf[i] == '+' || buf[i] == '-')
            {
                number[j] = '\0';

                unsigned long int conv_number = atoi(number);

                unsigned long int test = conv_number;
                for (int k = j - 1; k >= 0; k--)
                {
                    if(number[k] - '0' != test % 10)
                    {
                        error = true;
                        break;
                    }

                    test = test / 10;
                }

                if(sum > sum + conv_number || error == true)
                {
                    error = true;
                    break;
                }

                if(buf[i] == '+')
                {
                    sum = sum + conv_number;
                }
                else
                {
                    sum = sum - conv_number;
                }
                
                j = 0;

                if(buf[i] == '\n')
                {
                    data = true;
                    break;
                }
                
            }
            else if(buf[i] == ' ' || buf[i] == '\n' || (buf[i] == '\r' && buf[i + 1] == '\n'))
            {
                data = true;
                break;
            }
            else if(buf[i] >= '0' && buf[i] <= '9')
            {
                number[j] = buf[i];
                j++;

                if(j == 11)
                {
                    error = true;
                    break;
                }
            }
            else
            {
                error = true;
                break;
            }
        }

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
        }
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
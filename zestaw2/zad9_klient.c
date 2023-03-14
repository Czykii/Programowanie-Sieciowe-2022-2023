/*
Spróbuj napisać podobną parę klient-serwer komunikującą się za pomocą protokołu UDP. 
Pamiętaj, że UDP nie jest protokołem połączeniowym: wywołanie connect na gniazdku UDP nie powoduje wysłania w sieć żadnych pakietów. 
Klient musi jako pierwszy wysłać jakiś datagram, a serwer dowiaduje się o istnieniu klienta dopiero gdy ten datagram do niego dotrze. 
Sprawdź, czy możliwe jest wysyłanie pustych datagramów (tzn. o długości zero bajtów) — jeśli tak, to może niech klient właśnie taki wysyła?
*/

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>

bool printable_buf(const void * buf, int len){

    const char * buff = buf;

    for (int i = 0; i < len; i++)
    {
        if (*buff < 32 || *buff > 126)
        {
            return false;
        }
        
        ++buff;
    }
    
    return true;
}

int main(int argc, char* argv[])
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // na wyniki zwracane przez recvfrom() i sendto()
    const char* ip = argv[1];     //zmienna zawierająca adres ip
    int port = atoi(argv[2]);     //zmienna zawierająca numer portu

    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = inet_addr(ip) },   // 127.0.0.1
        .sin_port = htons(port)
    };

    unsigned char buf[15];
    memcpy(buf, "", 0);

    cnt = sendto(sock, NULL, 0, 0, (struct sockaddr *) & addr, sizeof(addr));
    if (cnt == -1) {
        perror("sendto");
        return 1;
    }
    //printf("sent %zi bytes\n", cnt);

    cnt = recvfrom(sock, buf, 15, 0, NULL, NULL);
    if (cnt == -1) {
        perror("recvfrom");
        return 1;
    }
    printf("received %zi bytes\n", cnt);

    if(printable_buf(buf, cnt) == false)
    {
        perror("Bajty mają znaki niedrukowalne");
        return 1;
    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
/*
Napisz prostego klienta, który łączy się (użyj socket i connect) z usługą wskazaną argumentami podanymi w linii komend 
(adres IPv4 w argv[1], numer portu TCP w argv[2]), drukuje na ekranie wizytówkę zwróconą przez serwer i kończy pracę. 
Pamiętaj o zasadzie ograniczonego zaufania i przed przesłaniem odebranego bajtu na stdout weryfikuj, czy jest to znak drukowalny 
lub znak kontrolny używany do zakończenia linii bądź wstawienia odstępu ('\n', '\r' oraz '\t').
*/

#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

int main(int argc, char* argv[])
{
    int sock;
    int rc;         // "rc" to skrót słów "result code"
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu
    char *p;
    int ip = strtol(argv[1], &p, 10);       //zmienna zawierająca numer ip
    int port = strtol(argv[2], &p, 10);     //zmienna zawierająca numer portu


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = htonl(ip) },   // 127.0.0.1
        .sin_port = htons(port)
    };

    rc = connect(sock, (struct sockaddr *) & addr, sizeof(addr));
    if (rc == -1) {
        perror("connect");
        return 1;
    }

    unsigned char buf[15];
    memcpy(buf, "ping", 4);

    cnt = read(sock, buf, 15);
    if (cnt == -1) {
        perror("read");
        return 1;
    }
    printf(cnt);
    printf("read %zi bytes\n", cnt);

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdbool.h>
#include<sys/time.h>

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

    struct timeval timeout = {
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
    }

    unsigned char buf[15];
    //memcpy(buf, "", 0);

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
    else
    {
        printf("%s\n",buf);
    }

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
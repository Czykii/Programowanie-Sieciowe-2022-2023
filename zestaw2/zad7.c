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
    ssize_t cnt;    // wyniki zwracane przez read() i write() są tego typu
    const char* ip = argv[1];     //zmienna zawierająca adres ip
    int port = atoi(argv[2]);     //zmienna zawierająca numer portu


    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        perror("socket");
        return 1;
    }

    struct sockaddr_in addr = {
        .sin_family = AF_INET,
        .sin_addr = { .s_addr = inet_addr(ip) },   // 127.0.0.1
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

    if(printable_buf(buf, cnt) == false)
    {
        perror("Bajty mają znaki niedrukowalne");
        return 1;
    }

    printf("%s\n",buf);

    rc = close(sock);
    if (rc == -1) {
        perror("close");
        return 1;
    }

    return 0;
}
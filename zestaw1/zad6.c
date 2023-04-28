#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

int main(int argc, char* argv[]){

    int in = open(argv[1], O_RDONLY);
    if(in == -1){
        perror("Blad otwarcia pliku wejscia");
        exit(1);
    }

    int out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC, 0666);
    if(out == -1){
        perror("Blad otwarcia pliku wyjscia");
        exit(1);
    }

    size_t readd;
    size_t written;
    char buf[64];

    while(1){
        readd = read(in, &buf, sizeof(buf));
        if(readd == -1){
            perror("Blad czytania");
            exit(1);
        }
        else if (readd == 0)
        {
            break;
        }
        else{
            written = write(out, &buf, readd);
            if(written == -1){
                perror("Blad zapisu");
                exit(1);
            }
        }
    }

    if(close(in) == -1 || close(out) == -1)
    {
        perror("Blad zamkniecia plikow");
    }

    return 0;
}
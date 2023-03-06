/*
Zaimplementuj program kopiujący dane z pliku do pliku przy pomocy powyższych funkcji.
Zakładamy, że nazwy plików są podawane przez użytkownika jako argumenty programu (tzn. będą dostępne w tablicy argv).
Zwróć szczególną uwagę na obsługę błędów — każde wywołanie funkcji we-wy musi być opatrzone testem sprawdzającym,
czy zakończyło się ono sukcesem, czy porażką.

Funkcje POSIX zwracają -1 aby zasygnalizować wystąpienie błędu,
i dodatkowo zapisują w globalnej zmiennej errno kod wskazujący przyczynę wystąpienia błędu
(na dysku nie ma pliku o takiej nazwie, brak wystarczających praw dostępu, itd.). 
Polecam Państwa uwadze pomocniczą funkcję perror, która potrafi przetłumaczyć ten kod na zrozumiały dla człowieka komunikat 
i wypisać go na ekranie.
*/

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

    int out = open(argv[2], O_CREAT | O_WRONLY | O_TRUNC);
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
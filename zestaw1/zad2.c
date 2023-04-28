#include <stdio.h>

void drukuj_alt(int * tablica, int liczba_elementow){

    int * p = tablica;
    
    while (p < tablica + liczba_elementow)
    {
        if (*p > 10 && *p < 100)
        {
           printf("%d\n", *p);
        }
        
        ++p;
    }   
}

int main(){

    int liczby[50];
    int i = 0;
    int in;

    for (i; i < 50; i++)
    {
        printf("Podaj liczbe: ");
        scanf("%d", &in);

        if (in == 0)
        {
            break;
        }

        liczby[i] = in;
    }

    printf("Przerwano petle\n");

    drukuj_alt(liczby, i);

    return 0;
}
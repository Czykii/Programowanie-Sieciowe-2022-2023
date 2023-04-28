#include<stdio.h>

void drukuj(int tablica[], int liczba_elementow){

    printf("Liczby mniejsze niz 100 i wieksze niz 10: \n");

    for (int i = 0; i < liczba_elementow; i++)
    {
        if(tablica[i] > 10 && tablica[i] < 100){
            printf("%d \n", tablica[i]);       
        }
    }
}

int main(){
    int liczby[50];
    int in;
    int i;

    for (i = 0; i < 50; i++)
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

    drukuj(liczby, i);
    
    return 0;
}
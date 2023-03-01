/*
Napisz program w C deklarujący w funkcji main tablicę int liczby[50] i wczytujący do niej z klawiatury kolejne liczby. 
Wczytywanie należy przerwać gdy użytkownik wpisze zero albo gdy skończy się miejsce w tablicy (tzn. po wczytaniu 50 liczb).

Z main należy następnie wywoływać pomocniczą funkcję drukuj, przekazując jej jako argumenty adres tablicy oraz liczbę wczytanych do niej liczb.
Funkcję tę zadeklaruj jako void drukuj(int tablica[], int liczba_elementow). 
W jej ciele ma być pętla for drukująca te elementy tablicy, które są większe od 10 i mniejsze od 100.
*/

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
/*
A teraz opracuj wersję, która jako argument dostaje łańcuch w sensie języka C, 
czyli ciąg niezerowych bajtów zakończony bajtem równym zero (ten końcowy bajt nie jest uznawany za należący do łańcucha).
Ta wersja funkcji powinna mieć taką sygnaturę: bool printable_str(const char * buf).
*/

#include <stdio.h>
#include <stdbool.h>

bool printable_str(const char * buf){

    while(true){
        if(*buf == 0){
            return true;
        }
        if(*buf < 32 || *buf > 126){
            return false;
        }
        ++buf;
    }    
}

int main(){

    const char buf[13] = "AleFajneSlowo";
    
    if (printable_str(buf) == true)
    {
        printf("Slowo jest drukowalne\n");
    }
    else
    {
        printf("Slowo nie jest drukowalne\n");
    }

    return 0;
}
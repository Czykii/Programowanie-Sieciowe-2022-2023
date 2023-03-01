/*
Opracuj funkcję sprawdzającą, czy przekazany jej bufor zawiera tylko i wyłącznie drukowalne znaki ASCII,
tzn. bajty o wartościach z przedziału domkniętego [32, 126]. Funkcja ma mieć następującą sygnaturę:
bool printable_buf(const void * buf, int len).

Pamiętaj o włączeniu nagłówka <stdbool.h>, bez niego kompilator nie rozpozna ani nazwy typu bool, ani nazw stałych true i false.

Trzeba będzie użyć rzutowania wskaźników, bo typ void * oznacza „adres w pamięci, 
ale bez informacji o tym co w tym fragmencie pamięci się znajduje”. 
Na początku ciała funkcji trzeba go zrzutować do typu „adres fragmentu pamięci zawierającego ciąg bajtów”.
*/

#include <stdio.h>
#include <stdbool.h>

bool printable_buf(const void * buf, int len){

    const char * buff = buf;

    

}

int main(){

    return 0;
}
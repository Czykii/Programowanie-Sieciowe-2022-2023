#include <stdio.h>
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

int main(){

    const char buf[13] = "AleFajneSlowo";

    if (printable_buf(buf, 13) == true)
    {
        printf("Slowo jest drukowalne\n");
    }
    else
    {
        printf("Slowo nie jest drukowalne\n");
    }
    
    return 0;
}
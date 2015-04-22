#include <stdio.h>
#define addr 0xffffffff80000000+0xb8100

int main(int argc, char* argv[], char* envp[])
{
    int cnt=0;
    for(int i=0; i<10; i++) {
        *((char*)addr + cnt++)=70;
        *((char*)addr + cnt++)=0x07;
    }
    while(1);
        //printf("%d. Hello\n", ++cnt);
    return 0;
}


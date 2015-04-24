#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8100

int main(int argc, char* argv[], char* envp[])
{
    write(1, "3\0", 1);
    while(1) {
        write(1, "Demon\n\0", 6);
    }
    /*
    int cnt=0;
    *((char*)addr + cnt++)=51;
    *((char*)addr + cnt++)=0x07;
    while(1) {
        for(int i=0; i<26; i++) {
            cnt = 2;
            *((char*)addr + cnt++)=65+i;
            *((char*)addr + cnt++)=0x07;
        }
    }
    */
    //printf("%d. Demon\n", ++cnt);
    return 0;
}

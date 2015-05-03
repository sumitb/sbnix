#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8040

int main(int argc, char* argv[], char* envp[])
{
    write(1, "4\0", 1);
    while(1) {
        write(1, "Lorde \0", 6);
    }
    //printf("%d. Lorde\n", ++cnt);
    return 0;
}

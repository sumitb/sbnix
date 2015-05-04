#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8040

int main(int argc, char* argv[], char* envp[])
{
    int cnt = 0;
    printf("%d. Lorde\n", ++cnt);
    while(1);
    return 0;
}

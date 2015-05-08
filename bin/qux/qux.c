#include <stdio.h>
#include <stdlib.h>
#define addr 0xffffffff80000000+0xb8040

int main(int argc, char* argv[], char* envp[])
{
    int cnt = 0;
    char **env, **arg;
    printf("%d. Lorde\n", argc);
    for (arg = argv; *arg != 0; arg++)
    {
        char* thisEnv = *arg;
        printf("%s\n", thisEnv);
    }
    for (env = envp; *env != 0; env++)
    {
        char* thisEnv = *env;
        printf("%s\n", thisEnv);
    }
    printf("%d. Lorde\n", ++cnt);
    while(1);
    return 0;
}

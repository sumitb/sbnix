#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/console.h>

int listdir(const char *path) {
    struct dirent *entry;
    DIR *dp;

    dp = opendir(path);
    if (dp == NULL) {
        printf("Error : Not able to open directory \n");
        return -1;
    }
   // entry = readdir(dp);
    while((entry = readdir(dp)) != NULL)
	{
        	printf("%s\n", entry->d_name);
	}
   while(1);  
    closedir(dp);
    return 0;
}

int main(int argc, char* argv[], char* envp[])
{
	char path[50];
	strcpy(path,getcwd(path,50));
	listdir(path);
	return 0;
}


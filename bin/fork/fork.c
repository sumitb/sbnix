#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/console.h>


int main(int argc, char* argv[], char* envp[]) {
   char *var = (char *)malloc(sizeof(200));
   strcpy(var,"Malloc rigouros testing along with fork and execve. Full proof plan");
 
   int k = fork();
   if(k == 0){
        printf("%s in child\n",var);
        execve("bin/qux",argv,envp);


   }
   else{
      waitpid(k,0,0);
       printf("\n Parent.\n");
       printf("%s in parent\n",var) ;
       strcpy(var,"modifying the value of test in parent");
       printf("%s\n",var);


}
    exit(0);
    return 0;
}


#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(){
    char buf[30];
    strcpy(buf,"he\'llo");
    for(int i=0; i<5; i++){
        printf("%c", buf[i]);
    }
    return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxArgs 50
#define charSize 512
#define cmdSize maxArgs * charSize
#define fileSize 1024
#define maxSize 4096
#define STDIN_FILENO 0

void execLine(char* input, char** argv, char** envp);

/* getline: get line into s, return length */
/*int getline(char s[], int lim)
{
    char c;
    int i=0;
    while(--lim>0 && (c=getchar()) != EOF && c != '\n')
        s[i++] = c;
    //if(c == '\n')
        //s[i++] = c;
    s[i] = '\0';
    return i;
}*/

char* last_path;
char* ps1;

/* Read single line from input including pipes */
int getline(char str[])
{
    char c;
    int i=0;
    do {
        scanf("%c", &c);
        str[i++] = c;
    } while(c != '\n' && c != EOF);
    str[i] = '\0';
    
    return 0;
}

/* Breaks input into 2-d array */
/* Breaks single command into 2-d array */
char** parseCmd(char *str, char delimiter)
{
	int len=0, i=0, j=0, k=0, skip=0;
    char** argv= malloc(maxArgs * sizeof(char *));
    memset(argv, 0, maxArgs * sizeof(char *));

    // Initialize first argument with null
    argv[0] = malloc(charSize * sizeof(char));
    memset(argv[0], '\0', charSize * sizeof(char));

    // Bug: Can't put white space before commands
    len = strlen(str);
	for(i=0; i<len; i++) {
        // remove leading and continuous whitespaces
        if(str[i] == ' ' && i == 0) {
            while(str[i+1] == ' ' || str[i+1] == '\t') i++;
        }
	    
        if(str[i] != delimiter && str[i] != '\n') {
            argv[k][j++] = str[i];
		}
        if(str[i] == delimiter || str[i] == '\n') {
            //printf("%d. ", k);
            //printf("%d. %d_%c<", k, i, str[i]);
            // Bug fix: Insert new line character after splitting
            if(delimiter == '|' && j != 0) {
                argv[k][j++] = '\n';
            }
            argv[k][j] = '\0';
			//printf("%s\n", argv[k]);
            k++; j=0;
			
            // Skip trailing whitespaces
            while(str[i+1] == ' ' || str[i+1] == '\t') i++;

            // Initialize next argument with null
            argv[k] = malloc(charSize * sizeof(char));
            memset(argv[k], '\0', charSize * sizeof(char));
		}
	}
    argv[k] = NULL;
    return argv;
}

/*
void parseString(char *str)
{
	int len=0, i=0, j=0, k=0;
    char** argv;
    char inputList[maxArgs][cmdSize];
	
    len = strlen(str);
	for(i=0; i<=len; i++) {
        if(str[i] != '|' && str[i] != '\n' && str[i] != '\0') {
			inputList[k][j] = str[i];
			j++;
		}
		if(str[i] == '|' || str[i] == '\n' || str[i] == '\0') {
			inputList[k][j] = '\0';
		    argv = parseCmd(inputList[k], ' ');
            execCmd(argv);
			printf("%d. %s\n", k, inputList[k]); k++; j=0;
		}
	}
}
*/

int parsePATH(char *envp[], char home[], char path[], char user[], char hostname[])
{
    int error, len, i=0,j=0;
    char* charPtr, readPtr;
    char temp[maxSize];

    //printf("Executable name is %s\n", argv[0]);
    //for(i=1; i<argc; i++)
        //printf("%s ", argv[i]);
    while(*envp != NULL) {
        len = strlen("HOME=");
        if(!strncmp(*(envp), "HOME=", len)) {
            memset(temp, '\0', strlen(*(envp))+1);
            strcpy(temp, *(envp));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                home[j]=temp[i];
            //printf("%d %s\n", strlen(temp), home);
            /*
            readPtr = (char*)home;
            charPtr = (char*)temp + 5;
            while(charPtr != NULL)
                *(readPtr++) = *(charPtr++);
            */
        }
        len = strlen("PATH=");
        if(!strncmp(*(envp), "PATH=", len)) {
            memset(temp, '\0', strlen(*(envp))+1);
            strcpy(temp, *(envp));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                path[j]=temp[i];
            //printf("%d %s\n", strlen(temp), path);
        }
        len = strlen("USER=");
        if(!strncmp(*(envp), "USER=", len)) {
            memset(temp, '\0', strlen(*(envp))+1);
            strcpy(temp, *(envp));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                user[j]=temp[i];
            //printf("%d %s\n", strlen(temp), user);
        }
        len = strlen("HOSTNAME=");
        if(!strncmp(*(envp), "HOSTNAME=", len)) {
            memset(temp, '\0', strlen(*(envp))+1);
            strcpy(temp, *(envp));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                hostname[j]=temp[i];
            //printf("%d %s\n", strlen(temp), hostname);
        }
        *(envp++);
        //printf("%d ", len);
        //printf("%s\n", *(envp++));
    }

    return 0;
}


/* Feature 1: cd command */
void changeDir(char* dirPath, const char* home)
{
    //TODO do not copy character pointer, always use strcpy
    // printf(">%s<\n", dirPath);
    if(dirPath == NULL) {
        chdir(home);
    }
    else {
        if(!strcmp(dirPath, "~") || !strcmp(dirPath, ""))
            strcpy(dirPath, home);
        if(!strcmp(dirPath, "-"))
		    strcpy(dirPath, last_path);
        if(chdir(dirPath))
            printf("%s: No such file or directory.\n", dirPath);
	}
    getcwd(last_path, fileSize);
}

/* Feature 2: execute binaries interactively */
void execBin(char* binary, char* path, char** argv, char** envp)
{
    int i, j, k, error=1, status;
    char* singlePath;
    pid_t childPID;

    singlePath = malloc(fileSize * sizeof(char));
    if((childPID = fork()) == 0) {
        
        // Relative Path
        // ls -lrt
        do {
            j = 0;
            memset(singlePath, '\0', fileSize);
            // TODO: Call parseCmd with delimiter ':'
            while(i<strlen(path) && path[i] != ':') {
                singlePath[j++] = path[i++];
            }
            i++;
            if(singlePath[j-1] != '/')
                singlePath[j++] = '/';
            
            k = 0;
            while(k<strlen(binary) && binary[k] != '\n' && binary[k] != EOF)
                singlePath[j++] = binary[k++];

            //printf("%s args=%s | %s |", singlePath, argv[0], argv[1], argv[2]);
            error = execve(singlePath, argv, envp);
            //printf("%d\n", error);
            
            if(!error)
                exit(0);
        } while(i<strlen(path) && path[i] != '\n' && path[i] != EOF);
    }
    else {
        waitpid(childPID, &status, 0);
    }
}

/* Feature 3: Execute scripts */
void execShell(char* file_name, char* argv[], char* envp[]) {
	FILE* fd_shell = NULL;
	int ch; int i, n=0; char buffer[200]="\0";
	
    //if(!(strcmp(file_name[0], " ")))
		//fd_shell = fopen(file_name[0], "r");
    printf("_%s_", file_name);
    if((strcmp(file_name, " ")))
		fd_shell = fopen(file_name, "r");
    
	if(fd_shell == NULL)
	{
		printf("!Error : Not able to open file\n");
	}
	else {
		while (!feof(fd_shell)) {
		    // TODO: Confirm use of fgetc
            for (ch = fgetc(fd_shell); ch != EOF && ch != '\n'; ch = fgetc(fd_shell)) {
				buffer[n++] = ch;
			}

			// null-terminate the string
			buffer[n++] = '\n';
			buffer[n] = '\0';
			
			printf("line : %s\n", buffer);
            execLine(buffer, argv, envp);
			memset(buffer,'\0',sizeof(buffer));n=0;
		}
	}
}

/* Feature 5: Set PATH and PS1*/
void setPATH(char* newPath, char* envp[])
{

}

void setPS1(char* new_ps, char* envp[])
{
	char home[fileSize], path[maxSize];
    char user[charSize], hostname[charSize];
	parsePATH(envp, home, path, user, hostname);
	//printf("new_ps : %s\n",new_ps);
	if(!strcmp(new_ps, "\\u"))
		strcpy(new_ps,user);
	if(!strcmp(new_ps, "\\h"))
		strcpy(new_ps,hostname);
	
	strcpy(ps1,new_ps);

}

int execCmd(char** cmd, char** argv, char** envp)
{
    char* input = cmd[0];
    char home[fileSize], path[maxSize];
    char user[charSize], hostname[charSize];
    
    parsePATH(envp, home, path, user, hostname);
    // Check if valid command, ignore null or blank
    if( strcmp(input, " ") && strlen(input) > 0) {
        
        if(!strcmp(input, "cd"))
            changeDir(cmd[1], home);
        else if(!strcmp(input, "exit"))
            exit(0);
        else if(!strcmp(input, "sbush"))
            execShell(cmd[1], argv, envp);
        else if(!strcmp(input, "set")) {
			if(!strncmp(cmd[1], "PS1",3))
				setPS1(cmd[3], envp);
			else
				setPATH(cmd[1], envp);
			}
        else {
            execBin(input, path, cmd, envp);
        }
        //printf("%s len(%d) ", input, strlen(input));
    }
    return 0;
}

void execute_pipe_cmd(char cmd_list[], int i, int cnt, char** argv, char** envp){
	char** cmd;int j,l,k,p;int status;
	pid_t childpid;
	int pfd1[2];
	int pfd2[2];int error=0;
	char home[fileSize], path[maxSize];
    char user[charSize], hostname[charSize];
	
    char* singlePath;
    singlePath = malloc(fileSize * sizeof(char));
    
	cmd = parseCmd(cmd_list, ' ');
	/* for(j=0; cmd[j] != NULL; j++)
		printf("cmd[%d]: %s\n",j,cmd[j]);
	j=0; */
	parsePATH(envp, home, path, user, hostname);
	
	if( strcmp(cmd[0], " ") && strlen(cmd[0]) > 0) {
		if (i % 2 != 0){
			pipe(pfd1);
		}else{
			pipe(pfd2);
		} 
		
		if ((childpid = fork()) == 0) {
			if (i == 0)
				dup2(pfd2[1],1);
			else if (i == cnt-1){
				if ((cnt) % 2 != 0){ // for odd number of commands
					dup2(pfd1[0],0);
				}else{ // for even number of commands
					dup2(pfd2[0],0);
				}
			}
			else{
				if (i % 2 != 0){
					dup2(pfd2[0],0);
					dup2(pfd1[1],1);
					}else{ // for even i
					dup2(pfd1[0],0);
					dup2(pfd2[1],1);
				}
			} 
			do {
				k = 0;
				memset(singlePath, '\0', fileSize);
				// TODO: Call parseCmd with delimiter ':'
				while(l<strlen(path) && path[l] != ':')	 {
					singlePath[k++] = path[l++];
				}
				l++;
				if(singlePath[k-1] != '/')
					singlePath[k++] = '/';
				
				p = 0;
				while(p<strlen(cmd[0]) && cmd[0][p] != '\n' && cmd[0][p] != EOF){
					singlePath[k++] = cmd[0][p++];}

				error = execve(singlePath, argv, envp);
				if(!error)
					break;
			} while(l<strlen(path) && path[l] != '\n' && path[l] != EOF);
		}
		if (i == 0)
			close(pfd2[1]);
		else if (i == cnt-1){
			if ((cnt) % 2 != 0){ // for odd number of commands
				close(pfd1[0]);
			}else{ // for even number of commands
				close(pfd2[0]);
			}
		}
		else{
			if (i % 2 != 0){
					close(pfd2[0]);
					close(pfd1[1]);
				}else{ // for even i
					close(pfd1[0]);
					close(pfd2[1]);
			}
		}
		waitpid(childpid,NULL, 0);
	}
}


void execLine(char* input, char** argv, char** envp)
{
    int i; int cnt=0;
    char** cmd, **cmdList;
    
    cmdList = parseCmd(input, '|');
	for(cnt=0; cmdList[cnt] != NULL; cnt++);
	
	if(cnt<=1 && cmdList[0] != "" && cmdList[0] != NULL) {
			cmd = parseCmd(cmdList[0], ' ');
		    execCmd(cmd, argv, envp);
	}
	else{
		for(i=0; cmdList[i] != NULL; i++) {
			execute_pipe_cmd(cmdList[i], i, cnt, argv, envp);
		}
	}
}

int main(int argc, char* argv[], char** envp)
{
    char input[cmdSize];
    
    // NotAFeature: Handle case when input is passed using argv of sbush
    ps1 = malloc(fileSize * sizeof(char));
	last_path = malloc(fileSize * sizeof(char));
	getcwd(last_path, fileSize);
	getcwd(ps1, fileSize);
    
    printf("Welcome to sbu shell!\n");
    while(1) {
        //getcwd(ps1, fileSize);
        printf("[%s]$ ", ps1);
        
        memset(input, '\0', cmdSize+1);
        //len = read(STDIN_FILENO, input, cmdSize);
        //input[len-1] = '\0'; //replace \n with \0
        getline(input);
        if(strcmp(input, "\n"))
            execLine(input, argv, envp);
    }
    return 0;
}

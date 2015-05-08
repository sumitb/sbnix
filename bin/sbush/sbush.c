#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define maxArgs 8
#define charSize 22
#define cmdSize maxArgs * charSize
#define fileSize 128
#define maxSize 256

void execLine(char* input, char** argv, char** envp);

char* last_path;
char* ps1;

char curr_shell_path[40];

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
	int len=0, i=0, j=0, k=0;
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

int parsePATH(char *envp[], char home[], char path[], char user[], char hostname[])
{
    int len, i=0,j=0;
    //char* charPtr;
    char temp[maxSize];

    //printf("Executable name is %s\n", argv[0]);
    //for(i=1; i<argc; i++)
    // printf("envp[1] : %s ", envp[1]);
    for(int z=0; z<maxArgs; z++) {
        len = strlen("HOME=");
        if(!strncmp(envp[z], "HOME=", len)) {
           // memset(temp, '\0', (strlen(envp[z])+1));
		    memset(temp, '\0', maxSize);
            strcpy(temp, (envp[z]));
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
        len = strlen("USER=");
        if(!strncmp((envp[z]), "USER=", len)) {
            memset(temp, '\0', maxSize);
			strcpy(temp, (envp[z]));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                user[j]=temp[i];
       //     printf("%d %s\n", strlen(temp), user);
        }
        
		len = strlen("PATH=");
        if(!strncmp((envp[z]), "PATH=", len)) {
            memset(temp, '\0', maxSize);
			strcpy(temp, (envp[z]));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                path[j]=temp[i];
            //printf("%d %s\n", strlen(temp), path);
        }
        // TODO: BUG: Username not working in PS1
        len = strlen("HOSTNAME=");
        if(!strncmp((envp[z]), "HOSTNAME=", len)) {
            memset(temp, '\0', maxSize);
			strcpy(temp, (envp[z]));
            for(i=len,j=0; i<strlen(temp); i++,j++)
                hostname[j]=temp[i];
            //printf("%d %s\n", strlen(temp), hostname);
        }
        //*(envp++);
        //printf("%d ", len);
        //printf("%s\n", *(envp++));
    }

    return 0;
}
/* Feature 1: cd command */
void changeDir(char* dirPath, char* home)
{
    char buffer[200]="\0";int i=0,j=0;
    if(dirPath == NULL) {
		getcwd(last_path, fileSize);
         chdir(home);
	//printf("curr path : %s\n",last_path);
    }
    else {
        if(!strcmp(dirPath, "~") || !strcmp(dirPath, ""))
            strcpy(dirPath, home);
        else if(!strcmp(dirPath, "-")){
			strcpy(dirPath, last_path);
		}
		else if(dirPath[0]=='~'){
			strcpy(buffer,home);
			int len1=strlen(buffer);
			int len2=strlen(dirPath);
			for(i=1,j=len1;i<len2;i++,j++)
				buffer[j]=dirPath[i];
			buffer[j]='\0';
			strcpy(dirPath,buffer);
		}
		getcwd(last_path, fileSize);
		 if(chdir(dirPath))
             printf("%s: No such directory.\n", dirPath);
	}
    
}

/* Feature 2: execute binaries interactively */
void execBin(char* binary, char* path, char** argv, char** envp, char *cmd1)
{
    int i=0, j, k; 
    int error=1;
    int  status;
    pid_t childPID;
    char bin_name[30];
    char cmd_path[30];
    strcpy(bin_name,binary);
    strcpy(cmd_path,path);
    if((childPID = fork()) == 0) {
        // Absolute Path
        // Done: handle binaries like ./hello and ./a.out
        //error = execve(bin_name, argv, envp);
        error = execve(bin_name, argv, envp);
        
    	char singlePath[256];
    	//singlePath = malloc(100 * sizeof(char));
        // Relative Path
        // ls -lrt
        do {
            j = 0;
            memset(singlePath, '\0', 256);
            //printf("bin_name : %s ,cmd_path : %s, singlePath :%s,i :%d, j:%d\n", bin_name, cmd_path, singlePath,i,j);
            // TODO: Call parseCmd with delimiter ':'
            while(i<strlen(cmd_path) && cmd_path[i] != ':') {
                singlePath[j++] = cmd_path[i++];
            }
            i++;
            if(singlePath[j-1] != '/')
                singlePath[j++] = '/';
            
            k = 0;
            while(k<strlen(bin_name) && bin_name[k] != '\n' && bin_name[k] != EOF)
                singlePath[j++] = bin_name[k++];

            //printf("%s args=%s | %s |", singlePath, argv[0], argv[1], argv[2]);
           // printf("bin_name : %s ,cmd_path : %s, singlePath :%s,i :%d, j:%d\n", bin_name, cmd_path, singlePath,i,j);
            error = execve(singlePath, argv, envp);
            //printf("%d\n", error);
            
            if(!error)
                exit(1);
        } while(i<strlen(cmd_path) && cmd_path[i] != '\n' && cmd_path[i] != EOF);
        printf("%s: command not found.\n", bin_name);
	exit(0);
    }
    else {
		if(cmd1 != NULL && strcmp(cmd1,"&")==0){
			
		}
		else{
			waitpid(childPID, &status, 0);
		}
    }
}

/* Feature 3: Execute scripts */
void execShell(char* file_name, char* argv[], char* envp[]) {
	uint64_t fd_shell=-1;
	char ch[2]; int n=0; char buffer[200]="\0";
	int len=strlen(file_name);
	char temp_file[len+2];
	memset(temp_file,'\0',len+2);
	
    if(strcmp(file_name, " ")){
		//if((file_name[0]=='.' && file_name[1]=='/') || (file_name[0]=='/'))
			fd_shell = open(file_name, 0);
	/*	else{
			strcpy(temp_file,"./");
			strcat(temp_file,file_name);
			fd_shell = open(temp_file, 0);
		}*/
	}
		
    
	if(!fd_shell)
	{
		printf("sbush: %s: No such file or directory\n", file_name);
	}
	else {
		while (read(fd_shell,ch,1)) {

			buffer[n] = ch[0];
	//		printf("%c",ch[0]);
            // TODO: Add support of positional params
			// null-terminate the string
			if(buffer[n++] == '\n'){
				buffer[n] = '\0';
		//		printf("line : %s\n", buffer);
				if(strcmp(buffer,"#!")!=0)
					execLine(buffer, argv, envp);
				memset(buffer,'\0',sizeof(buffer));n=0;
			}
		}
		//close(fd_shell);
	}
}

/* Feature 5: Set PATH and PS1*/
void setPATH(char* newPath, char* envp[])
{
	char home[fileSize], path[maxSize];
    char user[charSize], hostname[charSize];
	parsePATH(envp, home, path, user, hostname);
    int len=0;int i=0;
	int lpath=0;int j=0;int p=5;
   // char* charPtr, readPtr;
    char temp[maxSize];
	int len_path=0;
	
    memset(temp,'\0',maxSize);
	strcpy(temp, "PATH=");
	
	len_path=strlen(newPath);
	lpath=strlen(path);
	//printf("%s\n",path);
    //printf("Executable name is %s\n", argv[0]);
    //for(i=1; i<argc; i++)
        //printf("%s ", argv[i]);
    for(int z=0; z<maxArgs; z++) {
        len = strlen("PATH=");
        if(!strncmp((envp[z]), "PATH=", len)) {
            //memset(temp, '\0', strlen(*(envp))+1);
			for(i=0;i<len_path;i++,p++){
				if(newPath[i]=='$'){
					for(j=0;j<lpath;j++){
						temp[p]=path[j];p++;
					}
					i+=5;
				}
				temp[p]=newPath[i];
			}
			temp[p]='\0';
            //strcat(temp, path);
			//strcat(temp, newPath);
			//printf("envp :%s\n",temp);
			strcpy((envp[z]), temp);
            //printf("%d %s\n", strlen(temp), path);
        }
        //*(envp++);
    }
}

void setPS1(char* new_ps, char* envp[], char *new_path)
{
	int i=0, j=0, esc_flag=2;
	char *curr_path;
	char home[fileSize];
	char  path[maxSize];
    char user[charSize], hostname[charSize];
	
    curr_path = malloc(fileSize * sizeof(char));
	//printf("new_ps: %s\n",new_ps);
	getcwd(curr_path, fileSize);
	//printf("curr_path: %s\n",curr_path);
//	strcpy(curr_path,"/home/sbush");
//	strcpy(new_path,"/home/sbush");
	
	memset(new_path, '\0', fileSize);
	parsePATH(envp, home, path, user, hostname);
	//printf("user : %s\n",user);
	
    //abc\u@\h
	while(new_ps[i] != '\0') {
		if(new_ps[i] == '\\') {
			esc_flag=1;
		}
		else if(new_ps[i] == 'h' && esc_flag==1) {
			strcat(new_path,hostname);
			esc_flag=0;j=j+strlen(hostname);
		}
		else if(new_ps[i] == 'u' && esc_flag==1) {
			strcat(new_path,user);
			esc_flag=0;j=j+strlen(user);
		}
		else if(new_ps[i] == 'w' && esc_flag==1) {
			strcat(new_path,curr_path);
			esc_flag=0;j=j+strlen(curr_path);
		}
		else {
			new_path[j]=new_ps[i];
			j++;
		}
		i++;
	}
}

int execCmd(char** cmd, char** argv, char** envp)
{
    char* input = cmd[0];
    char home[fileSize], path[maxSize];
    char user[charSize], hostname[charSize];
	//strcpy(path,getcwd(path,25));
	//	strcpy(home,"bin/");
    parsePATH(envp, home, path, user, hostname);
    // Check if valid command, ignore null or blank
    if( strcmp(input, " ") && strlen(input) > 0) {
        
        if(!strcmp(input, "cd"))
            changeDir(cmd[1], home);
        else if(!strcmp(input, "exit"))
            exit(0);
        else if(!strcmp(input, "set")) {
            if(cmd[1] == NULL || cmd[2] == NULL)
                printf("set: Insufficient arguments.\n");
            else if(cmd[3] != NULL)
                printf("set: Too many arguments.\n");
            else {
                if(!strncmp(cmd[1], "PS1", 3))
                    //setPS1(cmd[3], envp);
                    strcpy(ps1, cmd[2]);
                else if(!strncmp(cmd[1], "PATH", 4))
                    setPATH(cmd[2], envp);
            }
        }
		else if(!strcmp(input, "sbh")){
			execShell(cmd[1], argv, envp);
		}
		else if(!strcmp(input, "echo")){
			if(cmd[1] != NULL){
				int len=strlen(cmd[1]);
				for(int i=0; i<len; i++){
					printf("%c", cmd[1][i]);
					}
			}
		}
		else if(strcmp(input, "kill")==0 && strcmp(cmd[1], "-9")==0){
			kill(atoi(cmd[2]),9);
		}
       else {
            execBin(input, path, cmd, envp,cmd[1]);
        }
        //printf("%s len(%d) ", input, strlen(input));
    }
    return 0;
}
int pfd[2];
void execute_pipe_cmd(char cmd_list[], int i, int cnt, char** argv, char** envp)
{
	char** cmd; int l=0,k,p; //int status;
	pid_t childpid;
	//int pfd2[2]; 
	int error=0;
	char home[fileSize];
	char path[maxSize];
	char cmd_chld[30];
	int i_chld=i;
	int cnt_chld=cnt;
    char user[charSize], hostname[charSize];

  //  char* singlePath;
   // singlePath = malloc(fileSize * sizeof(char));
    
	cmd = parseCmd(cmd_list, ' ');
	strcpy(cmd_chld,cmd[0]);
	//printf("cmd_chld :%s\n",cmd_chld);

	
	/* for(j=0; cmd[j] != NULL; j++)
		printf("cmd[%d]: %s\n",j,cmd[j]);
	j=0; */
	parsePATH(envp, home, path, user, hostname);
	//strcpy(path,"bin");
	
	if( strcmp(cmd[0], " ") && strlen(cmd[0]) > 0) {
	
	if (i_chld == 0){
		pipe(pfd);
	}
	int fd1=pfd[0];
	int fd2=pfd[1];
	
	/*	if (i_chld % 2 == 0){
			pipe(pfd1);
		}else{
			pipe(pfd2);
		} */
	
	//int pfd1[2];
	
		
		
	if ((childpid = fork()) == 0) {
			if (i_chld == 0){
					dup2(1,fd2);
					//printf("fd1 :%d, fd2: %d\n",fd1,fd2);
			}
			else if (i_chld == cnt_chld-1){
					dup2(0, fd1);
					dup2(fd2,1);
			}
			else{
					dup2(0,fd1);
					dup2(1,fd2);
			} 
			char singlePath[256];
			
			do {
				k = 0;
				memset(singlePath, '\0', 256);
				// TODO: Call parseCmd with delimiter ':'
				while(l<strlen(path) && path[l] != ':')	 {
					singlePath[k++] = path[l++];
				}
				l++;
				if(singlePath[k-1] != '/')
					singlePath[k++] = '/';
				
				p = 0;
				while(p<strlen(cmd_chld) && cmd_chld[p] != '\n' && cmd_chld[p] != EOF){
					singlePath[k++] = cmd_chld[p++];}

				error = execve(singlePath, argv, envp);
				if(!error)
					break;
			} while(l<strlen(path) && path[l] != '\n' && path[l] != EOF);
		exit(0);
		}
		waitpid(childpid, NULL, 0);
	}
	
/*	if( strcmp(cmd[0], " ") && strlen(cmd[0]) > 0) {
		if (i_chld % 2 != 0){
			pipe(pfd1);
		}else{
			pipe(pfd2);
		} 
		
		if ((childpid = fork()) == 0) {

  			char singlePath[256];
			if (i_chld == 0)
				dup2(pfd2[1],1);
			else if (i_chld == cnt_chld-1){
				if ((cnt_chld) % 2 != 0){ // for odd number of commands
					dup2(pfd1[0],0);
				}else{ // for even number of commands
					dup2(pfd2[0],0);
				}
			}
			else{
				if (i_chld % 2 != 0){
					dup2(pfd2[0],0);
					dup2(pfd1[1],1);
					}else{ // for even i
					dup2(pfd1[0],0);
					dup2(pfd2[1],1);
				}
			} 
			do {
				k = 0;
				memset(singlePath, '\0', 256);
				// TODO: Call parseCmd with delimiter ':'
				while(l<strlen(path) && path[l] != ':')	 {
					singlePath[k++] = path[l++];
				}
				l++;
				if(singlePath[k-1] != '/')
					singlePath[k++] = '/';
				
				p = 0;
				while(p<strlen(cmd_chld) && cmd_chld[p] != '\n' && cmd_chld[p] != EOF){
					singlePath[k++] = cmd_chld[p++];}

				error = execve(singlePath, argv, envp);
				if(!error)
					break;
			} while(l<strlen(path) && path[l] != '\n' && path[l] != EOF);
		exit(0);
		}
		if (i_chld == 0)
			close(pfd2[1]);
		else if (i_chld == cnt_chld-1){
			if ((cnt_chld) % 2 != 0){ // for odd number of commands
				close(pfd1[0]);
			}else{ // for even number of commands
				close(pfd2[0]);
			}
		}
		else{
			if (i_chld % 2 != 0){
					close(pfd2[0]);
					close(pfd1[1]);
				}else{ // for even i
					close(pfd1[0]);
					close(pfd2[1]);
			}
		}
		waitpid(childpid, NULL, 0);
	}*/
}


void execLine(char* input, char** argv, char** envp)
{
    int i; int cnt=0;
    char** cmd, **cmdList;
    
    // Skip new lines or empty lines
    if(strcmp(input, "\n") && strcmp(input, "")) {
        cmdList = parseCmd(input, '|');
        for(cnt=0; cmdList[cnt] != NULL; cnt++);
        
        //if(cnt<=1 && cmdList[0] != "" && cmdList[0] != NULL) {
        if(cnt<=1 && cmdList[0] != NULL) {
            if(strcmp(cmdList[0],"")){
                cmd = parseCmd(cmdList[0], ' ');
                execCmd(cmd, argv, envp);
            }
        }
        else{
            for(i=0; cmdList[i] != NULL; i++) {
                if(strcmp(cmdList[i], "\n") && strcmp(cmdList[i], "")){
		    //printf("cmdList[%d] : %s\n",i,cmdList[i]);
                    execute_pipe_cmd(cmdList[i], i, cnt, argv, envp);
		}
                else
                    printf("Invalid null command.\n");
            }
        }
    }
}

int main(int argc, char* argv[], char** envp1)
{
    char input[cmdSize];
	char *sh_target;
	int len_inp=0;
    
	//TODO free memory allocated by Malloc
   
	//set envp
	char **envp=(char **)malloc(8*sizeof(char *));
		for(int z=0;z<8;z++)
			envp[z]=(char *)malloc(512*sizeof(char));

	
 	strcpy(envp[0],"HOSTNAME=sbrocks");
 	strcpy(envp[1],"USER=root");
 	strcpy(envp[2],"HOME=bin");
 	strcpy(envp[3],"PATH=rootfs/bin/:bin/:mnt/");
	
    ps1 = malloc(fileSize * sizeof(char));
	last_path = malloc(fileSize * sizeof(char));
	sh_target = malloc(fileSize * sizeof(char));
	getcwd(last_path, fileSize);
    strcpy(ps1, "\\w");
    
    // Feature3: Handle case when input is passed using argv of sbush
    //if(argc > 1)
      //  execShell(argv[1], argv, envp);
    // Feature2: Run shell interactively
 //   else {
        printf("Welcome to sbu shell!\n");
        while(1) {
            setPS1(ps1, envp, sh_target);
            printf("[%s]$ ", sh_target);
            
            memset(input, '\0', cmdSize+1);
            scanf("%s", input);
			if(strcmp(input, "")) {
				len_inp = strlen(input);
                input[len_inp] = '\n';
                input[len_inp+1] = '\0';
               // getline(input);
                //if(strcmp(input, "\n"))
                execLine(input, argv, envp);
            }
        }
   // }
    return 0;
}

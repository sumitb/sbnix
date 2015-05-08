#include <sys/console.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/sched.h>



void tarfs_initialize(){
	struct posix_header_ustar *tarfs_str;
	tarfs_str=(struct posix_header_ustar *)&_binary_tarfs_start;
	int ind=0;
//	printk("tarfs_struct\n");
	while(tarfs_str < (struct posix_header_ustar *)&_binary_tarfs_end){
		strcpy(tarfs_ind[ind].name,tarfs_str->name);
//		printk("file_name : %s\n",tarfs_str->name);
	 	
		tarfs_ind[ind].ref_cnt=0;
		tarfs_ind[ind].file_size=atoo(tarfs_str->size);
		strcpy(tarfs_ind[ind].typeflag,tarfs_str->typeflag);
		
		tarfs_ind[ind].tarfs_start_addr=(uint64_t)tarfs_str++;
		tarfs_ind[ind].bin_start_addr=(uint64_t)tarfs_str;
		
		if( (tarfs_ind[ind].file_size%512)==0)
			tarfs_str += (tarfs_ind[ind].file_size/512);
		else
			tarfs_str += (tarfs_ind[ind].file_size/512)+1;
		ind++;
	}
}


uint64_t check_file(const char *file_name){
	int ind=0;
	for(ind=0;ind<MAX_BIN;ind++){
	if(strcmp(file_name,tarfs_ind[ind].name)==0){
		return tarfs_ind[ind].bin_start_addr;
		}
	}
	return 0;
}

char * sys_getcwd(char *dir_name, uint64_t size){
    struct task_struct *proc = getCurrentTask();
	/*int len = strlen(proc->bin_name);
	int i;int cnt=0;
	for(i=0; i<len; i++){
		if(proc->bin_name[i]=='/')
			cnt++;
	}
	for(i=0; i<len; i++){
		if(proc->bin_name[i]=='/' && cnt>0){
			cnt--;
		}
		if(cnt)
			dir_name[i]=proc->bin_name[i];
	}*/
	return proc->dir_path;
}

int64_t sys_chdir(char *path){
	int ind=0;
	struct task_struct *proc = getCurrentTask();
	int len = strlen(path);
	for(ind=0;ind<MAX_BIN;ind++){
		if((strncmp(path,tarfs_ind[ind].name,len)==0) && strcmp(tarfs_ind[ind].typeflag,DIR)==0){
			strcpy(proc->dir_path,path);
			return 0;
		}
	}
	return -1;
}

//uint64_t open(char *file_name){
int64_t sys_open(const char *file_name, int flags){
	int ind=0;
	int len=0;
	len=strlen(file_name);
	struct task_struct *process = getCurrentTask();
	for(ind=0;ind<MAX_BIN;ind++){
	if((strncmp(file_name,tarfs_ind[ind].name,len)==0) && ((tarfs_ind[ind].name[len]=='/' && tarfs_ind[ind].name[len+1]=='\0') || tarfs_ind[ind].name[len]=='\0')){	
	// && strcmp(tarfs_ind[ind].typeflag,FILE)==0){
        tarfs_ind[ind].ref_cnt++;
        process->fd_cnt++;
        process->fd[process->fd_cnt].offset=tarfs_ind[ind].bin_start_addr;
        process->fd[process->fd_cnt].flags=flags;
       	//strcpy(fd[file_fd].path,file_name);
       	strcpy(process->fd[process->fd_cnt].path,tarfs_ind[ind].name);
        process->fd[process->fd_cnt].seek=0;
	//	return tarfs_ind[ind].bin_start_addr;
        return process->fd_cnt;
		}
	}
	printk("No such file\n");
	return 0;
}
int64_t sys_getdents(int x,dirent* dirp,int count){
   // int x=fd;
    int i=0;
    int ind=0;
    uint64_t num_bytes=0;
     //dirent *arr=(dirent *)(KERN_MEM+mem_allocate());
     dirent *arr=dirp;
	 struct task_struct *process = getCurrentTask();
     int len=strlen(process->fd[x].path);
 
    // void * buff=malloc(4096);
    if(len>0){
    for(ind=0;ind<MAX_BIN;ind++){
         if(strncmp(process->fd[x].path,tarfs_ind[ind].name,len)==0 && (tarfs_ind[ind].name[len]!='\0')){
             if((strlen(tarfs_ind[ind].name))>len){
                strcpy(arr[i].d_name,tarfs_ind[ind].name);
                 arr[i].d_off=tarfs_ind[ind].bin_start_addr;
		 arr[i].d_reclen=sizeof(dirent);
          //   printk("%s\n",tarfs_ind[ind].name);
		num_bytes = num_bytes + sizeof(dirent);	
                i++;
              }
         }
     }
   }
     // buff=(void*)arr[i];
    // dirp=arr;
   return num_bytes;
 }

//void close(uint64_t addr){
int64_t sys_close(int32_t filedes) {
	int ind=0;
	struct task_struct *process = getCurrentTask();
	for(ind=0;ind<MAX_BIN;ind++){
//	if(addr==tarfs_ind[ind].bin_start_addr){
    	    if(tarfs_ind[ind].bin_start_addr==process->fd[filedes].offset){
    		tarfs_ind[ind].ref_cnt--;
    		process->fd[filedes].offset=0;
    		strcpy(process->fd[filedes].path," ");
    		process->fd[filedes].flags=0;
		return 1;
	    }
	}
	return 0;
}

/*int sys_closedir(void* dir_type){
	int ind=0;
    DIR *dir= (DIR *) dir_type;
	for(ind=0;ind<MAX_BIN;ind++){
	if(dir->offset==tarfs_ind[ind].bin_start_addr){
		tarfs_ind[ind].ref_cnt--;
        dir->status=0;
//        free(dir);
		return 1;
	    }
	}
    return 0;
}


void *opendir(char *dir){
	int ind=0;
    DIR* dir_type=malloc(sizeOf(DIR));
	int len = strlen(dir);
	for(ind=0;ind<MAX_BIN;ind++){
		if(strncmp(dir,tarfs_ind[ind].name,len)==0 && tarfs_ind[ind].name[len]=='/' && strcmp(tarfs_ind[ind].typeflag,DIR)==0){
		    strncpy(dir_type->dir_name,dir,NAME_MAX+1);
            tarfs_ind[ind].ref_cnt++;
            dir_type->offset=tarfs_ind[ind].bin_start_addr;   
            dir_type->fd=global_fd++;
            dir_type->status=1;
            dir_type->size=0;
	//		return tarfs_ind[ind].bin_start_addr;
	        return (void *)dir_type;
    }
	}
	printk("No such Directory\n");
	return NULL;
}




dirent *readdir(void *dir){
	int ind=0;
    int i=0;
//	int len = strlen(dir);
	DIR *dir_type=(DIR *) dir;
    int len=strlen(dir_type->dir_name);
    dirent *arr=(dirent *)malloc(4096); 
    memset(arr,'0',4096);
	for(ind=0;ind<MAX_BIN;ind++){
		if(strncmp(dir_type->dir_name,tarfs_ind[ind].name,len)==0 && tarfs_ind[ind].name[len]=='/'){
                                                 //   && strcmp(tarfs_ind[ind].typeflag,FILE)==0 && (strcmp(dir,tarfs_ind[ind].name))){
            if((strlen(tarfs_ind[ind].name))>len){
		        arr[i]->d_name=tarfs_[ind].name;
                arr[i]->d_off=tarfs_[ind].bin_start_addr;
         //   printk("%s\n",tarfs_ind[ind].name);
                i++;
		}
	}
}
	return arr;
}

ssize_t read(int filedes,void *buf, size_t count){
    int ind=0; int i=0;
	char *dest;
	char *src;
	dest =(char*)buf;
	for(ind=0;ind<MAX_BIN;ind++){
		if(fd[filedes]->offset==tarfs_ind[ind].bin_start_addr && strcmp(tarfs_ind[ind].typeflag,FILE)==0){
			if(tarfs_ind[ind].file_size<size)
				size=tarfs_ind[ind].file_size;
			src=(char*)tarfs_ind[ind].bin_start_addr;
			for(i=0; i<size;i++){
				dest[i]=*(src++);
			}
			dest[i]='\0';
			return size;
		
	    }
	}
	return 0;
}
*/




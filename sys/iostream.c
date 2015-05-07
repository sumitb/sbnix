#include <string.h>
#include <sys/sched.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/console.h>

int64_t sys_write(uint64_t fildes, char *buf, uint64_t size) {
	struct task_struct *process = getCurrentTask();
	if(process->dup_arr[fildes]!=fildes){
		//write to pipe buffer
		char *pipe_temp_buf=(char *)process->fd[process->dup_arr[fildes]].offset;
		int len=strlen(pipe_temp_buf);
		for(int i=0;i<size;i++,len++)
			pipe_temp_buf[len]=buf[i];
		return size;
	}
	else if(fildes==1 || fildes==2){
		char tem_buf[size];

		strncpy(tem_buf,buf,size);
		tem_buf[size] = '\0';
		return printk(tem_buf);
	}
    /*if(fildes==1 || fildes==2) {
    }
    return 1;
    */
	return 0;
}

int64_t sys_read(uint64_t fildes, char *buf, uint64_t size) {

    /*int i=0;
    char *dest;
    char *src;*/
    if(size>0){
		struct task_struct *process = getCurrentTask();
		if(process->dup_arr[fildes]!=fildes){
			//read from pipe buffer
			char *pipe_temp_buf=(char *)process->fd[process->dup_arr[fildes]].offset;
			strcpy(buf,pipe_temp_buf);
			return strlen(pipe_temp_buf);
		}
		else if(fildes==0){
            char tem_buf[size];
            tem_buf[size] = '\0';
            uint64_t len=0;
            len = scank(tem_buf);
            if(len>size)
                len=size;
            strncpy(buf,tem_buf,len);
            buf[len]='\0';
            return len;
        }
        /*
        if(fildes>2){
            if(fd[fildes].seek==0)
                src=(char*)fd[fildes].offset;
            else
                src=(char*)fd[fildes].seek;
            for(i=0; i<size;i++){
                dest[i]=*(src++);
            }
            dest[i]='\0';
            buf=dest;
            return size;
        }*/
    }
    return 0;
}

int64_t sys_lseek(int fildes,off_t offset,int whence){
    /*
    if(whence==0){    //set
       fd[fildes].seek=offset;
       return fd[fildes].seek;
    }
    if(whence==1){        //current
        fd[fildes].seek+=offset;
       return fd[fildes].seek;

    }

    if(whence==2){   //end
        int ind=0;
        while(strcmp(fd[fildes].path,tarfs_ind[ind].name)!=0){
            ind++;
        }
        fd[fildes].seek=tarfs_ind[ind].file_size+offset;
       return fd[fildes].seek;

    }*/
    return -1;
}

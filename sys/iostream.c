#include <string.h>
#include <sys/sched.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/console.h>

int64_t sys_write(uint64_t fildes, char *buf, uint64_t size) {
    char tem_buf[size];

    strncpy(tem_buf,buf,size);
    tem_buf[size] = '\0';
    return printk(tem_buf);
    /*if(fildes==1 || fildes==2) {
    }
    return -1;
    */
}

int64_t sys_read(uint64_t fildes, char *buf, uint64_t size) {

    int i=0;
    char *dest;
    char*src;
    if(fildes<=2){
        if(size>0){
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
        return 0;
    }
    else{
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
    }
}

off_t sys_lseek(int fildes,off_t offset,int whence){

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

    }
    return -1;
}


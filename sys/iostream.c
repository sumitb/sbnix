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


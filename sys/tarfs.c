#include <sys/console.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <string.h>

void tarfs_initialize(){
	struct posix_header_ustar *tarfs_str;
	tarfs_str=(struct posix_header_ustar *)&_binary_tarfs_start;
	int ind=0;
	printk("tarfs_struct\n");
	while(tarfs_str < (struct posix_header_ustar *)&_binary_tarfs_end){
		strcpy(tarfs_ind[ind].name,tarfs_str->name);
		printk("file_name : %s\n",tarfs_str->name);
	 	
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

uint64_t check_file(char *file_name){
	int ind=0;
	for(ind=0;ind<MAX_BIN;ind++){
	if(strcmp(file_name,tarfs_ind[ind].name)==0){
		return tarfs_ind[ind].bin_start_addr;
		}
	}
	return 0;
}
/*
uint64_t open(char *file_name){
	int ind=0;
	for(ind=0;ind<MAX_BIN;ind++){
	if(strcmp(file_name,tarfs_ind[ind].name)==0 && strcmp(tarfs_ind[ind].typeflag,FILE)==0){
		tarfs_ind[ind].ref_cnt++;
		return tarfs_ind[ind].bin_start_addr;
		}
	}
	printk("No such file\n");
	return 0;
}

void close(uint64_t addr){
	int ind=0;
	for(ind=0;ind<MAX_BIN;ind++){
	if(addr==tarfs_ind[ind].bin_start_addr){
		tarfs_ind[ind].ref_cnt--;
		break;
	    }
	}
}

uint64_t opendir(char *dir){
	int ind=0;
	int len = strlen(dir);
	for(ind=0;ind<MAX_BIN;ind++){
		if(strncmp(dir,tarfs_ind[ind].name,len)==0 && tarfs_ind[ind].name[len]=='/' && strcmp(tarfs_ind[ind].typeflag,DIR)==0){
			tarfs_ind[ind].ref_cnt++;
			return tarfs_ind[ind].bin_start_addr;
		}
	}
	printk("No such Directory\n");
	return 0;
}

void closedir(uint64_t addr){
	int ind=0;
	for(ind=0;ind<MAX_BIN;ind++){
	if(addr==tarfs_ind[ind].bin_start_addr){
		tarfs_ind[ind].ref_cnt--;
		break;
	    }
	}
}

uint64_t readdir(char *dir){
	int ind=0;
	int len = strlen(dir);
	
	for(ind=0;ind<MAX_BIN;ind++){
		if(strncmp(dir,tarfs_ind[ind].name,len)==0 && tarfs_ind[ind].name[len]=='/' && strcmp(tarfs_ind[ind].typeflag,FILE)==0){
			printk("%s\n",tarfs_ind[ind].name);
		}
	}
	return 0;
}

ssize_t read(uint64_t addr, int size, uint64_t buf){
	int ind=0; int i=0;
	char *dest;
	char *src;
	dest =(char*)buf;
	for(ind=0;ind<MAX_BIN;ind++){
		if(addr==tarfs_ind[ind].bin_start_addr && strcmp(tarfs_ind[ind].typeflag,FILE)==0){
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



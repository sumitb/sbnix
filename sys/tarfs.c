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
	 	tarfs_ind[ind].file_size=atoo(tarfs_str->size);
		tarfs_ind[ind].start_addr=tarfs_str++;
		if( (tarfs_ind[ind].file_size%512)==0)
			tarfs_str += (tarfs_ind[ind].file_size/512);
		else
			tarfs_str += (tarfs_ind[ind].file_size/512)+1;
		ind++;
	}
}


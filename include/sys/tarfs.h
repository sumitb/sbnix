#ifndef _TARFS_H
#define _TARFS_H
#include <stdlib.h>
extern char _binary_tarfs_start;
extern char _binary_tarfs_end;

#define DIR "5"
#define FILE "0"
#define MAX_BIN 20


struct posix_header_ustar {
	char name[100];
	char mode[8];
	char uid[8];
	char gid[8];
	char size[12];
	char mtime[12];
	char checksum[8];
	char typeflag[1];
	char linkname[100];
	char magic[6];
	char version[2];
	char uname[32];
	char gname[32];
	char devmajor[8];
	char devminor[8];
	char prefix[155];
	char pad[12];
};

struct tarfs {
	char name[100];
	int file_size;
	char typeflag[1];
	int ref_cnt;
	uint64_t tarfs_start_addr;
	uint64_t bin_start_addr;
	}tarfs_ind[MAX_BIN];

void tarfs_initialize();

/*uint64_t check_file(char *file_name);
uint64_t open(char *file_name);
void close(uint64_t addr);
uint64_t opendir(char *dir);
void closedir(uint64_t addr);
uint64_t readdir(char *dir);
ssize_t read(uint64_t addr, int size, uint64_t buf);
*/
#endif

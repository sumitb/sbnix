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

struct elf_header
{
  unsigned char e_ident[16];
  short int e_type;
  short int e_machine; 
  int e_version;
  uint64_t e_entry; 
  char e_phoff[8]; 
  char e_shoff[8]; 
  int e_flags; 
  short int e_ehsize;
  short int e_phentsize;
  short int e_phnum; 
  short int e_shentsize; 
  short int e_shnum; 
  short int e_shstrndx; 
};

struct pheader
{
 int  p_type; 
  int p_flags;
  uint64_t p_offset; 
  uint64_t p_vaddr; 
  char p_paddr[8]; 
  uint64_t p_filesz; 
  uint64_t p_memsz; 
  char p_align[8]; 
};





#endif

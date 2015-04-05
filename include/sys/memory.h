#ifndef __MEMORY_H
#define __MEMORY_H

#include <sys/defs.h>

#define AVAIL 0
#define BUSY 1
#define MAX_MEM 32510  //total 4K pages for  Memory [100000-7FFE000]
#define INITIAL_MEM 100000
#define KERN_MEM 0xffffffff80000000
#define END_LIMIT 0x7ffe000
#define VIDEO_MEM 0xb8000

#define PAGE_PERM 7
#define PAGE_PRESENT 1

#define PML4E 101
#define PDPE 102
#define PDE 103
#define PTE 104


struct memory_map {
                uint64_t addr;
                int res_flag;
        }__attribute__((packed));

struct memory_map memmap[MAX_MEM];


void* memset(void *str,int val,uint64_t size);

uint64_t mem_allocate();
void mem_free(uint64_t addr_t);
void init_memmap(void*  physfree);
void mem_init(void* physbase,void* physfree);

uint64_t page_roundoff_4096(uint64_t addr);
uint64_t addr_res(uint64_t logical, int flag);



uint64_t* walk_pages(uint64_t *pml4e,uint64_t logical);


void map_kernel(uint64_t *pml4e, uint64_t logical, uint64_t physical, uint64_t size);
#endif

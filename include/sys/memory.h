#ifndef __MEMORY_H
#define __MEMORY_H

#include <sys/defs.h>

#define AVAIL 0
#define BUSY 1
#define MAX_MEM 32510  //total 4K pages for  Memory [100000-7FFE000]
#define INITIAL_MEM 100000
#define KERN_MEM 0xffffffff80000000
#define STACK_MEM_TOP 0xf0000000
#define END_LIMIT 0x7ffe000
#define VIDEO_MEM 0xb8000

#define PAGE_PERM 7
#define PAGE_PRESENT 1

#define PML4E 101
#define PDPE 102
#define PDE 103
#define PTE 104


struct vm_area_struct {
    unsigned long vm_start;
    unsigned long vm_end;

    struct vm_area_struct *vm_next, *vm_prev;
    };

struct memory_map {
                uint64_t addr;      /* No need to store addr, can evaluate it */
                int res_flag;
        };

struct memory_map memmap[MAX_MEM];

uint64_t *pml4e;
uint64_t cr3_addr;

void* memset(void *str,int val,uint64_t size);

uint64_t mem_allocate();
void mem_free(uint64_t addr_t);
void init_memmap(void*  physfree);
void mem_init(void* physbase,void* physfree);
uint64_t kmalloc(size_t bytes);

uint64_t page_roundoff_4096(uint64_t addr);
uint64_t addr_res(uint64_t logical, int flag);

void page_fault();

uint64_t* walk_pages(uint64_t *pml4e,uint64_t logical);

void page_insert(uint64_t *pml4e, uint64_t logical, uint64_t physical);
void map_kernel(uint64_t *pml4e, uint64_t logical, uint64_t physical, uint64_t size);
void kmalloc_user_space(uint64_t *pml4e,uint64_t logical, uint64_t size);
#endif

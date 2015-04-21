#ifndef _PROCESS_H
#define _PROCESS_H

#include <sys/defs.h>
#include <sys/sched.h>

#define ELF_NIDENT	16

typedef struct vm_struct vma;

struct task{
    uint16_t parent_id;
    uint16_t process_id;
    uint64_t stack[64];
    uint64_t rsp_ptr;
    uint64_t cr3_address;
    uint64_t pml4e_addr;
    uint64_t rip_ptr;
    uint64_t entry_pt;
    vma *heap_vma;
    uint64_t kstack[512];
    uint8_t status;
    struct mm_struct *mm;
}__attribute__((packed));

struct run_queue{
	struct task process;
	struct run_queue *next;
}__attribute__((packed));


struct run_queue rqueue[2];
struct run_queue *rq_head;
int curr_proc;
struct run_queue running_proc;

typedef struct {
        unsigned char e_ident[ELF_NIDENT];
        uint16_t      e_type;
        uint16_t      e_machine;
        uint32_t      e_version;
        uint64_t      e_entry;
        uint64_t      e_phoff;
        uint64_t      e_shoff;
        uint32_t      e_flags;
        uint16_t      e_ehsize;
        uint16_t      e_phentsize;
        uint16_t      e_phnum;
        uint16_t      e_shentsize;
        uint16_t      e_shnum;
        uint16_t      e_shstrndx;
} __attribute__((packed))elf_header;

typedef struct {
        uint32_t   p_type;
        uint32_t   p_flags;
        uint64_t   p_offset;
        uint64_t   p_vaddr;
        uint64_t   p_paddr;
        uint64_t   p_filesz;
        uint64_t   p_memsz;
        uint64_t   p_align;
} __attribute__((packed))pheader;

struct task th1, th2, readyQueue[5];

uint16_t proc_cnt;
struct task kernel;

struct run_queue * create_process(char *binary);
void init_process(uint64_t *stack);
//static void elf_load(struct task *t, char *file_name);
//static void allocate(struct task *t,void * addr, int len);
vma* allocate_vma(struct vm_struct *vma_head);
void initialize_thread();

#endif

/*
typedef struct {
	uint32_t	sh_name;
	uint32_t	sh_type;
	uint32_t	sh_flags;
	uint64_t	sh_addr;
	uint64_t	sh_offset;
	uint32_t	sh_size;
	uint32_t	sh_link;
	uint32_t	sh_info;
	uint32_t	sh_addralign;
	uint32_t	sh_entsize;
} Elf64_Shdr;


*/

/*struct elfheader
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
}__attribute__((packed));

struct p_header
{
 int  p_type; 
  int p_flags;
  uint64_t p_offset; 
  uint64_t p_vaddr; 
  char p_paddr[8]; 
  uint64_t p_filesz; 
  uint64_t p_memsz; 
  char p_align[8]; 
}__attribute__((packed));


typedef struct p_header pheader;
typedef struct elfheader elf_header;
*/


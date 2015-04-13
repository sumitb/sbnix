#ifndef _PROCESS_H
#define _PROCESS_H

# define ELF_NIDENT	16

struct task{
uint16_t parent_id;
uint16_t process_id;
uint64_t *stack;
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

struct vm_struct{
    uint64_t vm_start;
    uint64_t vm_end;
    uint64_t vm_mmsz;
    uint64_t vm_file;
    struct vm_struct *vm_next;
    struct mm_struct *vm_mm;

}__attribute__(packed);

typedef struct vm_struct vma;
typedef struct task task;

struct mm_struct{

    vma *mmap;

}__attribute__(packed);


struct run_queue rqueue[2];
struct run_queue *rq_head;
//int curr_proc=0;
struct run_queue running_proc;
/*
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
} __attribute__((packed))Elf_hdr;

typedef struct {
        uint32_t   p_type;
        uint32_t   p_flags;
        uint64_t   p_offset;
        uint64_t   p_vaddr;
        uint64_t   p_paddr;
        uint64_t   p_filesz;
        uint64_t   p_memsz;
        uint64_t   p_align;
} Elf64_Phdr;

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

struct task th1,th2,readyQ[5];

uint16_t proc_cnt;
struct task kernel;



void create_process(char *binary);

#endif

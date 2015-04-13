#include <string.h>
#include <sys/process.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/console.h>


void funct1(){
printk("Hello\n");
}

void funct2(){
printk("World\n");
}

void load(struct task *th, char *file_name){
/*		uint64_t offset = check_file(file_name);
		//TODO check if file exists or not
		Elf_hdr *elf = (Elf_hdr *) (&_binary_tarfs_start + offset);
		Elf64_Phdr *pgmhdr;
		pgmhdr=(Elf64_Phdr *)((uint64_t)elf+elf->e_phoff);
		uint32_t pgm_cnt = elf->e_phnum;*/
}

void initialize_thread(){
uint64_t *page_addr1;
uint64_t *page_addr2;
uint64_t *pml4e_1;
uint64_t *pml4e_2;

page_addr1=(uint64_t *)mem_allocate();
page_addr2=(uint64_t *)mem_allocate();

pml4e_1=page_addr1 + KERN_MEM;
pml4e_2=page_addr2 + KERN_MEM;

pml4e_1[511]=pml4e[511];
pml4e_2[511]=pml4e[511];

th1.cr3_address=(uint64_t)page_addr1;
th2.cr3_address=(uint64_t)page_addr2;

th1.pml4e_addr=(uint64_t)pml4e_1;
th2.pml4e_addr=(uint64_t)pml4e_2;

load(&th1,"/bin/hello");
load(&th2,"/bin/hello1");

th1.stack[59]=(uint64_t)th1.entry_pt;
th1.rsp_ptr=(uint64_t)&th1.stack[45];

th2.stack[59]=(uint64_t)th2.entry_pt;
th2.rsp_ptr=(uint64_t)&th2.stack[45];

rqueue[0].process=th1;
rqueue[0].next=&rqueue[1];

rqueue[1].process=th2;
rqueue[1].next=&rqueue[0];
}

void create_process(char *binary){
	uint64_t *page_addr;
	uint64_t *pml4e_pr;
	page_addr=(uint64_t *)mem_allocate();
	struct run_queue *proc=(struct run_queue *)mem_allocate();
	proc->next=NULL;
	struct run_queue *curr=rq_head;
	while(curr->next != NULL)
		curr=curr->next;
	curr->next=proc;
	
	pml4e_pr=page_addr + KERN_MEM;
	memset(pml4e_pr,0,4096);
	
	pml4e_pr[511]=pml4e[511];
	
	proc->process.cr3_address=(uint64_t)page_addr;
	proc->process.process_id=proc_cnt++;
	proc->process.pml4e_addr=(uint64_t)pml4e_pr;
	
	running_proc=*proc;
}
void init_process(uint64_t *stack){
	
	kernel.stack=&(stack[0]);
	kernel.process_id=proc_cnt++;
	kernel.cr3_address=cr3_addr;
	kernel.pml4e_addr=(uint64_t)pml4e;
	
	
	
}



/*void create_process(char *binary){
	uint64_t offset;
	if(strcmp(binary,"")){
		offset = check_file(binary);
		if(offset == 0)
			printk("%s not present\n",binary);
		else
			printk("%s present at %p\n",binary,offset);
	}
	offset = open(binary);
	close(offset);
	offset = open("hell");
	close(offset);
	
	offset = opendir("bin");
	readdir("bin");
	closedir(offset);
	offset=mem_allocate();
	uint64_t off = offset+KERN_MEM;
	memset((uint64_t *)off,0,4096);
	
}*/


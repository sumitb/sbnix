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



static void elf_load(struct task *t, char *file_name){
	uint64_t offset = check_file(file_name);
	int start=0;
	int end=0;
	//TODO check if file exists or not
	if(offset!=0){
		
		elf_header *elf = (elf_header *) (&_binary_tarfs_start + offset);
		int len = elf->e_phnum;
		pheader *ph1;
		pheader *ph2;
	   
		for(int i=0;i<len;i++){
			ph1 = (pheader *)(elf + (elf->e_phoff +(elf->e_phentsize * i)));
			if(ph1->p_type==ELF_PROG_LOAD){
				allocate(t,(void *) ph1->p_vaddr, ph1->p_memsz);
				lcr3(t->cr3);
				memset((char*) ph1->p_vaddr,0,ph1->p_memsz);
				
				memcpy((char *) ph1->p_vaddr, (void *) elf + ph1->p_offset, ph1->p_filesz);
				lcr3(boot_cr3);        
				vma *vm = allocate_vma(t->mm);
				vm->vm_start = ph1->p_vaddr;
				vm->vm_end = ph1->p_vaddr + ph1->p_memsz;
				vm->vm_mmsz=ph1->p_memsz;
				vm->vm_next=NULL;
				vm->vm_file=(uint64_t)elf;
				start=vm->vm_start;
				end=vm->vm_end;
			
			}

		
		}


		t->entry=elf->e_entry;
		t->heap_vma = (vma *)(KERN_MEM+mem_allocate());
		vma* tmp = t->mm->mmap;
		while(tmp->vm_next != NULL)
			tmp=tmp->vm_next;

		 uint64_t val = ALIGN_DOWN((uint64_t)(tmp->vm_end + 0x1000));
		 t->heap_vma->vm_end=t->heap_vma->vm_start=val;
		 t->heap_vma->vm_mmsz=0x1000;

		allocate(t,(void *)t->heap_vma->vm_start,t->heap_vma->vm_mmsz);
		t->mm->mmap->vm_start=start;
		t->mm->mmap->vm_end=end;
	}

}


static void allocate(struct task *t,void * addr, int len){

   // int start = ROUNDDOWN((int)addr, PGSIZE);
   // int end = ROUNDUP((int)addr+len, PGSIZE);
   int start = page_roundoff_4096(addr);
   int end = page_roundoff_4096(addr+len);
    page* p;
    
    for(int i=start;i<end;i+=PGSIZE){
        if(!(p=page_alloc(ALLOC_ZERO)))
            kprintf("out of memory\n");
        else{
            if(page_insert....)
                kprint("out of memory\n");

        
        }
    
    
    }


}



 vma* allocate_vma(struct mm_struct* mm){
    char *node;
    vma* end;
    if(mm->mmap == NULL){
            node=(char *)(kernbase+mem_allocate());
            end=(struct vma *)node;
            mm->mmap=end;
            //mm->counti+=1;
            return (struct vma*)node;

        }
    else{
        end=mm->mmap;
        while(end->vm_next!=NULL)
            end=end->vm_next;
        node=sizeof(struct vma)+(char *)end;
        end->vm_next = (struct vma *)node;
        return (struct vma *)node;
    
    }

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


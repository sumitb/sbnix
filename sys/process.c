#include <string.h>
#include <sys/process.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/console.h>
#include <sys/gdt.h>

static void allocate(uint64_t pml4e_addr,void * addr, int len){
	uint64_t start;
   //start address should be round down to 4096
   if(((uint64_t)addr)%4096 !=0){
	start = page_roundoff_4096((uint64_t)addr);
	start=start-4096;
   }
   else{
	   start=(uint64_t)addr;
   }
   uint64_t end = page_roundoff_4096((uint64_t)(addr+len));
   uint64_t p,i;
   uint64_t cnt=0;
    
    for(i=start;i<end;i+=4096,cnt+=4096){
        if(!(p=mem_allocate()))
            printk("out of memory\n");
        else{
                page_insert((uint64_t *)pml4e_addr,start+cnt, p);
        }
    }
}

 vma* allocate_vma(vma *vma_head){
    vma* tail;
    if(vma_head == NULL){
            vma_head=(vma *)(KERN_MEM+mem_allocate());
            return vma_head;
        }
    else{
        tail=vma_head;
        while(tail->vm_next!=NULL)
            tail=tail->vm_next;
        tail->vm_next=(vma *)(sizeof(vma)+(char *)tail);
        return tail->vm_next;
    }
	return NULL;
}
volatile int gdb=0;
static void elf_load(struct task *t, char *file_name){
	uint64_t offset = check_file(file_name);
	if(offset!=0){
		elf_header *elf = (elf_header *)offset;
	    pheader *ph1=(pheader *)((uint8_t *)elf + elf->e_phoff);
		int ph_cnt=elf->e_phnum;

		for(int i=0;i<ph_cnt;i++,ph1++){
			if(ph1->p_type==1){
				allocate(t->pml4e_addr,(void *) ph1->p_vaddr, ph1->p_memsz);
				__asm __volatile("movq %0,%%cr3" : : "r" (t->cr3_address));
				memset((char*) ph1->p_vaddr,0,ph1->p_memsz);
				memcpy((char *) ph1->p_vaddr, (void *) elf + ph1->p_offset, ph1->p_filesz);
				__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));       
				vma *vm = allocate_vma(t->mm->vma_addr);
				vm->vm_start = ph1->p_vaddr;
				vm->vm_end = ph1->p_vaddr + ph1->p_memsz;
				vm->vm_mmsz=ph1->p_memsz;
				vm->vm_next=NULL;
				vm->vm_file=(uint64_t)elf;
				vm->vm_offset=ph1->p_offset;
				//start=vm->vm_start;
				//end=vm->vm_end;
			}
		}

		t->entry_pt=elf->e_entry;
		/*t->heap_vma = (vma *)(KERN_MEM+mem_allocate());
		vma* tmp = t->mm->mmap;
		while(tmp->vm_next != NULL)
			tmp=tmp->vm_next;

		 uint64_t val = ALIGN_DOWN((uint64_t)(tmp->vm_end + 0x1000));
		 t->heap_vma->vm_end=t->heap_vma->vm_start=val;
		 t->heap_vma->vm_mmsz=0x1000;

		allocate(t,(void *)t->heap_vma->vm_start,t->heap_vma->vm_mmsz);
		t->mm->mmap->vm_start=start;
		t->mm->mmap->vm_end=end;*/
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

elf_load(&th1,"/bin/hello");
elf_load(&th2,"/bin/hello1");

th1.stack[59]=(uint64_t)th1.entry_pt;
th1.rsp_ptr=(uint64_t)&th1.stack[45];

th2.stack[59]=(uint64_t)th2.entry_pt;
th2.rsp_ptr=(uint64_t)&th2.stack[45];

rqueue[0].process=th1;
rqueue[0].next=&rqueue[1];

rqueue[1].process=th2;
rqueue[1].next=&rqueue[0];
}

struct run_queue * create_process(char *binary){
	uint64_t *page_addr;
	uint64_t *pml4e_pr;
	page_addr=(uint64_t *)mem_allocate();
	struct run_queue *proc=(struct run_queue *)(KERN_MEM + mem_allocate());
	//allocate 3 pages for process struct, change this later
	mem_allocate();
	mem_allocate();
	proc->next=NULL;
	struct run_queue *curr=rq_head;
	if(curr != NULL){
	while(curr->next != NULL)
		curr=curr->next;
	curr->next=proc;
	}
	else{
		curr=proc;
	}
	pml4e_pr=(uint64_t *)((uint64_t)page_addr + KERN_MEM);
	memset(pml4e_pr,0,4096);
	
	pml4e_pr[511]=pml4e[511];
	
	proc->process.cr3_address=(uint64_t)page_addr;
	proc->process.process_id=proc_cnt++;
	proc->process.pml4e_addr=(uint64_t)pml4e_pr;
	
	proc->process.mm=(struct mm_struct *)((char *)(&proc->process + 1));
	proc->process.mm->cnt=0;
	proc->process.mm->vma_addr=NULL;
	
	//proc->process.stack[63]= GDT_DS | P | W | DPL3,  /*** user data segment descriptor ***/
	//proc->process.stack[62]= (uint64_t)(&proc->process.stack[63]);
	//proc->process.stack[61]= 0x246;                           //  EFlags
//	proc->process.stack[60]= GDT_CS | P | DPL3 | L,  /*** user code segment descriptor ***/
	
	//proc->process.rsp_ptr = (uint64_t)(&proc->process.stack[63]);
	
        /*proc->process.kstack[506] = 0;
		proc->process.kstack[505] = 0; 
		proc->process.kstack[504] = 0; 
		proc->process.kstack[503] = 0;
        proc->process.kstack[502] = 0; 
		proc->process.kstack[501] = 0; 
		proc->process.kstack[500] = 0; 
		proc->process.kstack[499] = 0;
        proc->process.kstack[498] = 0;
		proc->process.kstack[497] = 0;
		proc->process.kstack[496] = 0; 
		proc->process.kstack[495] = 0;
        proc->process.kstack[494] = 0; 
		proc->process.kstack[493] = 0; 
		proc->process.kstack[492] = 0;
*/
		memset(proc->process.kstack,'\0',4096);
		//proc->process.kstack[491] = (uint64_t)(&irq0+34);
        proc->process.rsp_ptr = (uint64_t)&proc->process.kstack[490];

		proc->process.kstack[511] = 0x23 ;                              //SS    
        proc->process.kstack[510] = (uint64_t)(&proc->process.stack[511]);      //  ESP
        proc->process.kstack[509] = 0x246;                           // EFLAGS
        proc->process.kstack[508] = 0x1b ;                           //CS
        
		elf_load(&(proc->process), binary);	
        //proc->process.heap_vma->vm_end = proc->process.heap_vma->vm_start;
		proc->process.kstack[507] = (uint64_t)proc->process.entry_pt;  //RIP
//	__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
	running_proc=proc;
	return proc;
	
	
}
void init_process(uint64_t *stack){
	
	//kernel.stack=(uint64_t)&(stack[0]);
	kernel.process_id=proc_cnt++;
	kernel.cr3_address=cr3_addr;
	kernel.pml4e_addr=(uint64_t)pml4e;
	
	struct run_queue *proc = create_process("bin/hello");
	while(gdb);
	__asm __volatile("movq %0, %%cr3":: "a"(proc->process.cr3_address));
	//__asm __volatile("movq %0,%%cr3" : : "r" (proc->process.cr3_address));
	__asm__ __volatile__ (
            "movq %0, %%rsp;" //load next's stack in rsp
            :
            :"r"(proc->process.rsp_ptr)
    );
	tss.rsp0 = (uint64_t)&(proc->process.stack[511]);
	__asm__ __volatile__("popq %r15");
	__asm__ __volatile__("popq %r14");
	__asm__ __volatile__("popq %r13");
	__asm__ __volatile__("popq %r12");
	__asm__ __volatile__("popq %r11");
    __asm__ __volatile__("popq %r10");
	__asm__ __volatile__("popq %r9");
	__asm__ __volatile__("popq %r8");
	__asm__ __volatile__("popq %rdi");
	__asm__ __volatile__("popq %rsi");
	__asm__ __volatile__("popq %rdx");
	__asm__ __volatile__("popq %rcx");
	__asm__ __volatile__("popq %rbx");
	__asm__ __volatile__("popq %rax");
	//__asm__ __volatile__("sti");	

	
	__asm__ __volatile__ ("mov $0x2b,%ax");
  	__asm__ __volatile__ ("ltr %ax");
	__asm__ __volatile__ ("add $8, %rsp");
	__asm__ __volatile__ ("add $8, %rsp");
	__asm__ __volatile__ ("add $8, %rsp");
	__asm__ __volatile__("iretq");
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

uint16_t sys_fork(){
	struct run_queue *pproc = running_proc;
	struct run_queue *cproc = (struct run_queue *)(KERN_MEM + mem_allocate());
	mem_allocate();
	mem_allocate();
	if((uint64_t)cproc){
		uint64_t page_addr = (uint64_t )mem_allocate();
		uint64_t *pml4e_chld = (uint64_t *)(KERN_MEM + page_addr);
		memset(pml4e_chld,0,4096);
	
		pml4e_chld[511]=pml4e[511];
		
		cproc->process.cr3_address=page_addr;
		cproc->process.parent_id=pproc->process.process_id;
		cproc->process.process_id=proc_cnt++;
		cproc->process.pml4e_addr=(uint64_t)pml4e_chld;
		
		cproc->process.mm=(struct mm_struct *)((char *)(&cproc->process + 1));
		cproc->process.mm->cnt=0;
		cproc->process.mm->vma_addr=NULL;
		
		uint64_t *stack_page_physical=(uint64_t *)(mem_allocate());
		uint64_t *stack_page_virtual=(uint64_t *)(KERN_MEM + (void *)stack_page_physical);
		int i=0;
	//	int j=0;
		for(i=0; i<512; i++){
			stack_page_virtual[i]=pproc->process.stack[i];
	//		cproc->process.kstack[j]=pproc->process.kstack[j];
		}
		
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->process.cr3_address));
		
		//copy stack
		map_kernel(pml4e_chld,(uint64_t)cproc->process.stack,(uint64_t)stack_page_physical,4096);
			
		memset(cproc->process.kstack,'\0',4096);
		//proc->process.kstack[491] = (uint64_t)(&irq0+34);
        cproc->process.rsp_ptr = (uint64_t)&cproc->process.kstack[490];
		for(i=0;i<512;i++){
			cproc->process.kstack[i]=pproc->process.kstack[i];
		}

		//cproc->process.kstack[511] = 0x23 ;                              //SS    
          cproc->process.kstack[510] = (uint64_t)(&cproc->process.stack[511]);      //  ESP
		cproc->process.kstack[509] = 0x200286;                           // EFLAGS - 0x200286
        //cproc->process.kstack[508] = 0x1b ;                           //CS
        
		//copy vma
		vma *p_vm = pproc->process.mm->vma_addr;
		
		while(p_vm != NULL){
			vma *c_vm = allocate_vma(cproc->process.mm->vma_addr);
			c_vm->vm_start = p_vm->vm_start;
			c_vm->vm_end = p_vm->vm_end;
			c_vm->vm_mmsz=p_vm->vm_mmsz;
			c_vm->vm_next=p_vm->vm_next;
			c_vm->vm_file=p_vm->vm_file;
			c_vm->vm_offset=p_vm->vm_offset;
			allocate(cproc->process.pml4e_addr,(void *) c_vm->vm_start, c_vm->vm_mmsz);
			memset((char *) c_vm->vm_start,0,c_vm->vm_mmsz);
			memcpy((char *) c_vm->vm_start, (void *) (p_vm->vm_file + p_vm->vm_offset), c_vm->vm_mmsz);
			p_vm=p_vm->vm_next;
		}
		
		uint16_t child_id=cproc->process.process_id;
		//cproc->process.kstack[507] = pproc->process.kstack[507];
		cproc->process.kstack[506] =0;  // assigning rax of child to zero;
		cproc->process.rsp_ptr = (uint64_t)(&cproc->process.kstack[490]);
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->process.cr3_address));
		
		return child_id;
		//copy elf
		//push register
		//iretq
	}
	return 0;
}


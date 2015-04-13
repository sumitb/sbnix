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





static void allocate(struct task *t,void * addr, int len){
	uint64_t start;
   // int start = ROUNDDOWN((int)addr, PGSIZE);
   // int end = ROUNDUP((int)addr+len, PGSIZE);
   //TODO : start address should be round down to 4096
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
                page_insert((uint64_t *)t->pml4e_addr,start+cnt, p);
        
        }
    }
}



 vma* allocate_vma(struct mm_struct* mm){
    char *node;
    vma* end;
    if(mm->vma_addr == NULL){
            node=(char *)(KERN_MEM+mem_allocate());
            end=(vma *)node;
            mm->vma_addr=end;
            //mm->counti+=1;
            return (vma*)node;

        }
    else{
        end=mm->vma_addr;
        while(end->vm_next!=NULL)
            end=end->vm_next;
        node=sizeof(vma)+(char *)end;
        end->vm_next = (vma *)node;
        return (vma *)node;
    
    }
	return NULL;

}
volatile int gdb=1;
static void elf_load(struct task *t, char *file_name){
	uint64_t offset = check_file(file_name);
	//int start=0;
	//int end=0;
	//TODO check if file exists or not
	while(gdb);
	if(offset!=0){
		
		elf_header *elf = (elf_header *)offset;
//		int len = elf->e_phnum;
	//	pheader *ph1;
	    pheader *ph1=(pheader *)((uint8_t *)elf + elf->e_phoff);
      //  pheader *ph2;
	   pheader *ph2=ph1+elf->e_phnum;

//		for(int i=0;i<len;i++){
            for(;ph1<ph2;ph1++){
                
		//	ph1 = (pheader *)((uint8_t *)elf + (elf->e_phoff +(elf->e_phentsize * i)));
			if(ph1->p_type==1){
				allocate(t,(void *) ph1->p_vaddr, ph1->p_memsz);
				__asm __volatile("movq %0,%%cr3" : : "r" (t->cr3_address));
				memset((char*) ph1->p_vaddr,0,ph1->p_memsz);
				
				memcpy((char *) ph1->p_vaddr, (void *) elf + ph1->p_offset, ph1->p_filesz);
				__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));       
				vma *vm = allocate_vma(t->mm);
				vm->vm_start = ph1->p_vaddr;
				vm->vm_end = ph1->p_vaddr + ph1->p_memsz;
				vm->vm_mmsz=ph1->p_memsz;
				vm->vm_next=NULL;
				vm->vm_file=(uint64_t)elf;
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

void create_process(char *binary){
	uint64_t *page_addr;
	uint64_t *pml4e_pr;
	page_addr=(uint64_t *)mem_allocate();
	struct run_queue *proc=(struct run_queue *)(KERN_MEM + mem_allocate());
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
	
        proc->process.kstack[506] = 1; proc->process.kstack[505] = 2;  proc->process.kstack[504] = 3;  proc->process.kstack[503] = 4;
        proc->process.kstack[502] = 5; proc->process.kstack[501] = 6;  proc->process.kstack[500] = 7;  proc->process.kstack[499] = 8;
        proc->process.kstack[498] = 9; proc->process.kstack[497] = 10; proc->process.kstack[496] = 11; proc->process.kstack[495] = 12;
        proc->process.kstack[494] = 13; proc->process.kstack[493] = 14; proc->process.kstack[492] = 15;

        //proc->process.kstack[491] = (uint64_t)(&irq0+34);
        proc->process.rsp_ptr = (uint64_t)&proc->process.kstack[490];

		proc->process.kstack[511] = 0x23 ;                              //  Data Segment    
        proc->process.kstack[510] = (uint64_t)(&proc->process.stack[511]);      //  RIP
        proc->process.kstack[509] = 0x246;                           //  EFlags
        proc->process.kstack[508] = 0x1b ;                              // Code Segment
        
		elf_load(&(proc->process), binary);	
        //proc->process.heap_vma->vm_end = proc->process.heap_vma->vm_start;
		proc->process.kstack[507] = (uint64_t)proc->process.entry_pt;
	__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
	running_proc=*proc;
}
void init_process(uint64_t *stack){
	
	kernel.stack=&(stack[0]);
	kernel.process_id=proc_cnt++;
	kernel.cr3_address=cr3_addr;
	kernel.pml4e_addr=(uint64_t)pml4e;
	
	create_process("bin/hello");
	
	
	__asm__ __volatile__ ("mov $0x2b,%ax");
  	__asm__ __volatile__ ("ltr %ax");
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


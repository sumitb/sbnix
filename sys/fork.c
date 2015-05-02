#include <string.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>


uint16_t sys_fork(){
    uint64_t pproc_rsp;
    __asm__ __volatile__("movq %%rsp, %0":"=g"(pproc_rsp)::"memory");
	struct task_struct *pproc = getCurrentTask();
	//struct task_struct *cproc = (struct task_struct *)(KERN_MEM + mem_allocate());
	struct task_struct *cproc = (struct task_struct *)kmalloc(10*4096);
	/*mem_allocate();
	mem_allocate();
	mem_allocate();
	mem_allocate();
	mem_allocate();
	mem_allocate();
	mem_allocate();
	mem_allocate();
	mem_allocate();
	*/memset((void *)cproc,'\0',10*4096);
	if((uint64_t)cproc){
		__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
		uint64_t page_addr = (uint64_t )mem_allocate();
		uint64_t *pml4e_chld = (uint64_t *)(KERN_MEM + page_addr);
		memset(pml4e_chld,0,4096);
	
		pml4e_chld[511]=pml4e[511];
		
		addTasktoQueueHead(cproc);
		cproc->cr3_address=page_addr;
		cproc->ppid=pproc->pid;
		cproc->pid=++avail_pid;
		cproc->pml4e_addr=(uint64_t)pml4e_chld;
		
		cproc->mm=(struct mm_struct *)(KERN_MEM + mem_allocate());
		cproc->mm->cnt=0;
		cproc->mm->vma_addr=NULL;
		
		uint16_t child_id=cproc->pid;
		
		cproc->stack=(uint64_t*)STACK_MEM_TOP;
		//kmalloc_user_space(process->pml4e_addr,STACK_MEM,512*64)
		
		uint64_t *stack_page_virtual=(uint64_t *)kmalloc((sizeof(uint64_t)*(USER_STACK_SIZE)));
		uint64_t *stack_page_physical=(uint64_t *)((void *)stack_page_virtual-KERN_MEM);
		int i=0;int k=0;
		int j = 0;
		for(j = 0,k=511; j < 512; j++,k--) {
			uint64_t* dest = stack_page_virtual + j;
			uint64_t* src = pproc->stack - k;
			dest = src;
			src = dest;
		}
		
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
		//copy stack
		map_kernel(pml4e_chld,(uint64_t)cproc->stack - (sizeof(uint64_t)*(USER_STACK_SIZE)),
			(uint64_t)stack_page_physical,(sizeof(uint64_t)*(USER_STACK_SIZE)));
		__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
		
		//for(i=0; i<USER_STACK_SIZE; i++){
		//	cproc->stack[i]=pproc->stack[i];
	//		process->kstack[j]=pproc->process->kstack[j];
		//}
		vma *p_vm = pproc->mm->vma_addr;
		//copy vma
		while(p_vm != NULL){
			vma *c_vm = allocate_vma(&(cproc->mm->vma_addr));
			c_vm->vm_start = p_vm->vm_start;
			c_vm->vm_end = p_vm->vm_end;
			c_vm->vm_mmsz=p_vm->vm_mmsz;
			//c_vm->vm_next=p_vm->vm_next;
			c_vm->vm_file=p_vm->vm_file;
			c_vm->vm_offset=p_vm->vm_offset;
			allocate(cproc->pml4e_addr,(void *) c_vm->vm_start, c_vm->vm_mmsz);
			
			__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
			memset((char *) c_vm->vm_start,0,c_vm->vm_mmsz);
			memcpy((char *) c_vm->vm_start, (void *) (p_vm->vm_file + p_vm->vm_offset), c_vm->vm_mmsz);
			__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
			p_vm=p_vm->vm_next;
		}
		
		cproc->entry_pt=pproc->entry_pt;
		
		//proc->process->kstack[491] = (uint64_t)(&irq0+34);
		for(i=0;i<KERNEL_STACK_SIZE;i++){
			cproc->kstack[i]=pproc->kstack[i];
		}

		//process->kstack[511] = 0x23 ;                              //SS
        //cproc->kstack[KERNEL_STACK_SIZE-2] = (uint64_t)((cproc->stack)+((sizeof(uint64_t))*(USER_STACK_SIZE-1)));      //  ESP
		cproc->kstack[KERNEL_STACK_SIZE-2] = (uint64_t)STACK_MEM_TOP-0x8;      //  ESP
		cproc->kstack[KERNEL_STACK_SIZE-3] = 0x200286;                           // EFLAGS - 0x200286
        //process->kstack[508] = 0x1b ;                           //CS
        
		//process->kstack[507] = pproc->process->kstack[507];
		cproc->kstack[KERNEL_STACK_SIZE-5] = cproc->kstack[KERNEL_STACK_SIZE-10];      //  ESP
		cproc->kstack[KERNEL_STACK_SIZE-6] =0;  // assigning rax of child to zero;
		cproc->kernel_rsp = (uint64_t *)(&cproc->kstack[KERNEL_STACK_SIZE-22]);	//-50
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(pproc->cr3_address));
		
		return child_id;
		//copy elf
		//push register
		//iretq
	}
	return 0;
}


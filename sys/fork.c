#include <string.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>


uint16_t sys_fork(){
	struct task_struct *pproc = currentTask;
	struct task_struct *cproc = (struct task_struct *)(KERN_MEM + mem_allocate());
	mem_allocate();
	mem_allocate();
	memset((void *)cproc,'\0',4096);
	if((uint64_t)cproc){
		uint64_t page_addr = (uint64_t )mem_allocate();
		uint64_t *pml4e_chld = (uint64_t *)(KERN_MEM + page_addr);
		memset(pml4e_chld,0,4096);
	
		pml4e_chld[511]=pml4e[511];
		
		cproc->cr3_address=page_addr;
		cproc->ppid=pproc->pid;
		cproc->pid=avail_pid++;
		cproc->pml4e_addr=(uint64_t)pml4e_chld;
		
		cproc->mm=(struct mm_struct *)(KERN_MEM + mem_allocate());
		cproc->mm->cnt=0;
		cproc->mm->vma_addr=NULL;
		
		uint64_t *stack_page_physical=(uint64_t *)(mem_allocate());
		uint64_t *stack_page_virtual=(uint64_t *)(KERN_MEM + (void *)stack_page_physical);
		int i=0;
	//	int j=0;
		for(i=0; i<63; i++){
			stack_page_virtual[i]=pproc->stack[i];
	//		process->kstack[j]=pproc->process->kstack[j];
		}
		
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
		
		//copy stack
		map_kernel(pml4e_chld,(uint64_t)cproc->stack,(uint64_t)stack_page_physical,4096);
			
		
		//proc->process->kstack[491] = (uint64_t)(&irq0+34);
        cproc->kernel_rsp = (uint64_t *)&cproc->kstack[42];
		for(i=0;i<63;i++){
			cproc->kstack[i]=pproc->kstack[i];
		}

		//process->kstack[511] = 0x23 ;                              //SS    
          cproc->kstack[62] = (uint64_t)(&cproc->stack[63]);      //  ESP
		cproc->kstack[61] = 0x200286;                           // EFLAGS - 0x200286
        //process->kstack[508] = 0x1b ;                           //CS
        
		//copy vma
		vma *p_vm = pproc->mm->vma_addr;
		
		while(p_vm != NULL){
			vma *c_vm = allocate_vma(&(cproc->mm->vma_addr));
			c_vm->vm_start = p_vm->vm_start;
			c_vm->vm_end = p_vm->vm_end;
			c_vm->vm_mmsz=p_vm->vm_mmsz;
			c_vm->vm_next=p_vm->vm_next;
			c_vm->vm_file=p_vm->vm_file;
			c_vm->vm_offset=p_vm->vm_offset;
			allocate(cproc->pml4e_addr,(void *) c_vm->vm_start, c_vm->vm_mmsz);
			memset((char *) c_vm->vm_start,0,c_vm->vm_mmsz);
			memcpy((char *) c_vm->vm_start, (void *) (p_vm->vm_file + p_vm->vm_offset), c_vm->vm_mmsz);
			p_vm=p_vm->vm_next;
		}
		
		uint16_t child_id=cproc->pid;
		//process->kstack[507] = pproc->process->kstack[507];
		cproc->kstack[58] =0;  // assigning rax of child to zero;
		cproc->kernel_rsp = (uint64_t *)(&cproc->kstack[42]);
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
		
		return child_id;
		//copy elf
		//push register
		//iretq
	}
	return 0;
}


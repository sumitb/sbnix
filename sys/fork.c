#include <string.h>
#include <sys/tarfs.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/syscall.h>
#include <sys/console.h>


int64_t sys_execve(const char *filename,char *const argv[],char *const envp[]) {
	/* File type is not ELF */
    if(!check_file(filename))
        return -1;
    // Copy pid
    struct task_struct *exec_child = create_process(filename);
	struct task_struct *exec_parent = (struct task_struct*)getCurrentTask();
    --avail_pid;
    exec_child->pid = exec_parent->pid;
	exec_child->ppid = exec_parent->ppid;
	exec_parent->ppid = 0;
	
	//copy fd table
	exec_child->fd_cnt=exec_parent->fd_cnt;
	strcpy(exec_child->dir_path, exec_parent->dir_path);
	for(int i=0; i<=exec_parent->fd_cnt;i++){
		exec_child->fd[i].offset=exec_parent->fd[i].offset;
		strcpy(exec_child->fd[i].path,exec_parent->fd[i].path);
	}
	//copy dup table
	for(int i=0;i<25;i++){
		exec_child->dup_arr[i]=exec_parent->dup_arr[i];
	}
    addTasktoQueueHead(exec_child);
    sys_exit(0);
    return 0;
}

int64_t sys_fork() {
    //uint64_t pproc_rsp;
    //__asm__ __volatile__("movq %%rsp, %0":"=g"(pproc_rsp)::"memory");
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
        cow_walk_pages((uint64_t*)pproc->pml4e_addr, pml4e_chld);
		
		cproc->mm=(struct mm_struct *)(KERN_MEM + mem_allocate());
		cproc->mm->cnt=0;
		cproc->mm->vma_addr=NULL;

        strcpy(cproc->bin_name, (const char*)pproc->bin_name);
        strcpy(cproc->dir_path, (const char*)pproc->dir_path);
        cproc->is_sleep=false;
        cproc->sleep_time=0;
		uint16_t child_id=cproc->pid;
		
		//copy user stack
		cproc->stack=(uint64_t*)STACK_MEM_TOP;
		//kmalloc_user_space(process->pml4e_addr,STACK_MEM,512*64)
		
		/*uint64_t *stack_page_virtual=(uint64_t *)kmalloc((sizeof(uint64_t)*(USER_STACK_SIZE)));
		uint64_t *stack_page_physical=(uint64_t *)((void *)stack_page_virtual-KERN_MEM);
        __asm__ __volatile__ ("movq %0, %%cr3":: "a"(pproc->cr3_address));
        
        uint64_t *temp = pproc->stack - USER_STACK_SIZE;
        for(int i=0; i < USER_STACK_SIZE-1; i++) {
			stack_page_virtual[i] = temp[i];
		}
		
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
		map_kernel(pml4e_chld,(uint64_t)cproc->stack - (sizeof(uint64_t)*(USER_STACK_SIZE)),
			(uint64_t)stack_page_physical,(sizeof(uint64_t)*(USER_STACK_SIZE)));
		__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));*/
		
		//copy vma
		vma *p_vm = pproc->mm->vma_addr;
		while(p_vm != NULL){
			vma *c_vm = allocate_vma(&(cproc->mm->vma_addr));
			c_vm->vm_start = p_vm->vm_start;
			c_vm->vm_end = p_vm->vm_end;
			c_vm->vm_mmsz=p_vm->vm_mmsz;
			c_vm->vm_file=p_vm->vm_file;
			c_vm->vm_offset=p_vm->vm_offset;
		    /*
            allocate(cproc->pml4e_addr,(void *) c_vm->vm_start, c_vm->vm_mmsz);
			__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
			memset((char *) c_vm->vm_start,0,c_vm->vm_mmsz);
			memcpy((char *) c_vm->vm_start, (void *) (p_vm->vm_file + p_vm->vm_offset), c_vm->vm_mmsz);
			__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
            */
			p_vm=p_vm->vm_next;
		}
		
		cproc->entry_pt=pproc->entry_pt;
		//copy fd table
		cproc->fd_cnt=pproc->fd_cnt;
		for(int i=0; i<=pproc->fd_cnt;i++){
			cproc->fd[i].offset=pproc->fd[i].offset;
			strcpy(cproc->fd[i].path,pproc->fd[i].path);
		}
		for(int i=0;i<25;i++){
		cproc->dup_arr[i]=i;
		}
		//copy heap
		cproc->heap.bump_ptr=page_roundoff_4096(pproc->heap.bump_ptr);
		/*uint64_t size = pproc->heap.bump_ptr - BUMP_PTR;
		if(size){
			uint64_t *heap_page_virtual=(uint64_t *)kmalloc(pproc->heap.bump_ptr - BUMP_PTR);
			uint64_t *heap_page_physical=(uint64_t *)((void *)heap_page_virtual-KERN_MEM);
			__asm__ __volatile__ ("movq %0, %%cr3":: "a"(pproc->cr3_address));
			
			uint64_t *temp_heap = (uint64_t*)BUMP_PTR;
			for(int i=0; i < size; i++) {
				heap_page_virtual[i] = temp_heap[i];
			}
			
			__asm__ __volatile__ ("movq %0, %%cr3":: "a"(cproc->cr3_address));
			map_kernel(pml4e_chld, BUMP_PTR, (uint64_t)heap_page_physical, pproc->heap.bump_ptr - BUMP_PTR);
			__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));
		}*/
		//proc->process->kstack[491] = (uint64_t)(&irq0+34);
		for(int i=STACK_OFFSET; i<STACK_OFFSET+NUM_REGISTERS_SAVED+1; i++) {
			cproc->kstack[i]=pproc->kstack[i+1];
		}

		//process->kstack[511] = 0x23 ;                              //SS
        //cproc->kstack[KERNEL_STACK_SIZE-2] = (uint64_t)((cproc->stack)+((sizeof(uint64_t))*(USER_STACK_SIZE-1)));      //  ESP
        //process->kstack[508] = 0x1b ;                           //CS
        
		//process->kstack[507] = pproc->process->kstack[507];
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-6] = 0;  // assigning rax of child to zero;
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-5] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-5];                           // EIP
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-4] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-4];                           // CS
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-3] = 0x200286;                           // EFLAGS - 0x200286
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-2] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-2];                           // ESP
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-1] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-1];                           // SS
		cproc->kernel_rsp = (uint64_t *)(&cproc->kstack[STACK_OFFSET]);	        //-50
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(pproc->cr3_address));
		
		return child_id;
		//copy elf
		//push register
		//iretq
	}
	return 0;
}


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
    --avail_pid;
	struct task_struct *exec_parent = (struct task_struct*)getCurrentTask();
    exec_child->pid = exec_parent->pid;
	exec_child->ppid = exec_parent->ppid;
	exec_parent->ppid = 0;
	
	//copy fd table
	exec_child->fd_cnt=exec_parent->fd_cnt;
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
	struct task_struct *pproc = getCurrentTask();
	struct task_struct *cproc = (struct task_struct *)kmalloc(10*4096);
	memset((void *)cproc,'\0',10*4096);
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
        cproc->is_sleep=false;
        cproc->sleep_time=0;
		uint16_t child_id=cproc->pid;
		
		cproc->stack=(uint64_t*)STACK_MEM_TOP;
		
		//copy vma
		vma *p_vm = pproc->mm->vma_addr;
		while(p_vm != NULL){
			vma *c_vm = allocate_vma(&(cproc->mm->vma_addr));
			c_vm->vm_start = p_vm->vm_start;
			c_vm->vm_end = p_vm->vm_end;
			c_vm->vm_mmsz=p_vm->vm_mmsz;
			c_vm->vm_file=p_vm->vm_file;
			c_vm->vm_offset=p_vm->vm_offset;
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

		cproc->heap.bump_ptr=page_roundoff_4096(pproc->heap.bump_ptr);

		for(int i=STACK_OFFSET; i<STACK_OFFSET+NUM_REGISTERS_SAVED+1; i++) {
			cproc->kstack[i]=pproc->kstack[i+1];
		}
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-6] = 0;  // assigning rax of child to zero;
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-5] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-5];                           // EIP
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-4] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-4];                           // CS
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-3] = 0x200286;                           // EFLAGS - 0x200286
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-2] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-2];                           // ESP
		cproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-1] = pproc->kstack[KERNEL_STACK_SIZE-TSS_OFFSET-1];                           // SS
		cproc->kernel_rsp = (uint64_t *)(&cproc->kstack[STACK_OFFSET]);	        //-50
		__asm__ __volatile__ ("movq %0, %%cr3":: "a"(pproc->cr3_address));
		
		return child_id;
	}
	return 0;
}


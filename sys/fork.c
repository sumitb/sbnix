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


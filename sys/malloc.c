#include <sys/sched.h>
#include <sys/memory.h>

vma* allocate_vma(vma **vma_head){
    vma* tail;
    if(*vma_head == NULL){
            *vma_head=(vma *)(KERN_MEM+mem_allocate());
            return *vma_head;
        }
    else{
        tail=*vma_head;
        while(tail->vm_next!=NULL)
            tail=tail->vm_next;
        tail->vm_next=(vma *)(sizeof(vma)+(char *)tail);
        return tail->vm_next;
    }
	return NULL;
}

int64_t sys_brk(uint64_t bump_addr){
	struct task_struct *proc = getCurrentTask();
	//static uint64_t bump_ptr=BUMP_PTR;
	//static uint64_t rem_size=0; //remaining size in page after allocating memory
	if(bump_addr==0)
		return proc->heap.bump_ptr;
	else{
		uint64_t size;
		size = bump_addr - proc->heap.bump_ptr;
		uint64_t ret_addr;
		ret_addr = proc->heap.bump_ptr;
		uint64_t blk_cnt=0;
		if(size>0){
			if(proc->heap.rem_size>size){  //available size is greater than requested
				proc->heap.rem_size = proc->heap.rem_size-size;
				proc->heap.bump_ptr = proc->heap.bump_ptr+size;
				return ret_addr;
			}
			else{   //available is less than requested, allocate more pages
				uint64_t less_mem=0;
				uint64_t *pml4e_addr;
				uint64_t *pte;
				uint64_t physical;
				uint64_t logical;
				logical = proc->heap.bump_ptr + proc->heap.rem_size;
				//pml4e_addr=(uint64_t *)currentTask->pml4e_addr;
				pml4e_addr=(uint64_t *)proc->pml4e_addr;
				less_mem = size - proc->heap.rem_size;  //required memory to be allocated = requested - remaining
				blk_cnt = less_mem/4096;
				if((size%4096)>0)
					blk_cnt++;
				while(blk_cnt){
						physical=mem_allocate();
						pte=walk_pages(pml4e_addr,logical);
						if(*pte & PAGE_PRESENT)
							mem_free(*pte);
						*pte=(physical | PAGE_PERM);
						logical = logical + 4096;
						proc->heap.rem_size = proc->heap.rem_size+4096; //remaining memory increase by 4096 after eacch page allocation
						blk_cnt--;
				}
				proc->heap.rem_size = proc->heap.rem_size-size; //allocate requested memory and decrease remaining memory
				proc->heap.bump_ptr = proc->heap.bump_ptr+size;
				return ret_addr;
			}
		}
	}
	return 0;
}


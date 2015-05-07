#include <sys/sched.h>
#include <sys/console.h>
#include <sys/memory.h>
#include <sys/syscall.h>

void OOM(){
    printk("Out of Memory\n");
    sys_exit(0);
}

void segfault(uint64_t addr){
    printk("SIGSEGV: Segmentation fault at %p\n", addr);
    sys_exit(0);
}

int page_fault(uint64_t error){
	uint64_t *pte;
	uint64_t *pml4e_addr;
	uint64_t logical;
	uint64_t physical;

	__asm__ __volatile__ ("movq %%cr2, %0" : "=r"(logical));
    pml4e_addr=(uint64_t *)((struct task_struct*)getCurrentTask())->pml4e_addr;

	if(error & PAGE_PRESENT){
        if(error & PAGE_WRITE){
            uint64_t* pte = look_pages(pml4e_addr, logical);
            physical = *pte;
            physical = (physical & (0xFFFFFFFFFFFFF000));
            struct memory_map *memmap_ent = get_physical_memmap(physical);
            if(memmap_ent->cow_flag){
                if(memmap_ent->ref_cnt == 1) {
                    *pte=(physical | PAGE_PERM);
                }
                else{
                    memmap_ent->ref_cnt--;
                    uint64_t page_physical=mem_allocate();
                    uint64_t *page_virtual = (uint64_t*)(page_physical+KERN_MEM);
                    uint64_t *fault_virtual = (uint64_t*)(physical+KERN_MEM);

                    for(int i=0, j=0; i < PAGE_SIZE-1; i+=8, j++) {
                        page_virtual[j] = fault_virtual[j];
                    }

                    map_kernel(pml4e_addr, logical, page_physical, PAGE_SIZE);
                }
                return 0;
            }
            printk("Write access violation!\n");
            segfault(logical);
        }
        else {
            printk("Read access violation!\n");
            segfault(logical);
        }
        /* Freeing physical pages which aren't mapped :P */
        pte=walk_pages(pml4e_addr,logical);
        if(*pte & PAGE_PRESENT)
            mem_free(*pte);
    }
    else{
        //TODO: Alloc if addr in heap or stack
        //Currently exit if it's a kernel address
        if(logical < INITIAL_MEM){
            segfault(logical);
        }
        else{
            pte=walk_pages(pml4e_addr,logical);
            physical=mem_allocate();
            *pte=(physical | PAGE_PERM);
        }
    }
    return -1;
}


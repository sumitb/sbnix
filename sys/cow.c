#include <sys/memory.h>

int64_t cow_walk_pages(uint64_t *pml4e, uint64_t *pml4e_child){
	uint64_t *pdpe, *pdpe_child;
	uint64_t pdpe_t;
	uint64_t *pde, *pde_child;
	uint64_t pde_t;
	uint64_t *pte, *pte_child;
	uint64_t pte_t;
	uint64_t page_addr_pml4e_child;
	uint64_t page_addr_pdpe_child;
	uint64_t page_addr_pde_child;
	
    /* Level 4 */
	for (int i=0; i<511; i++){
        if(pml4e[i] & PAGE_PRESENT){
            page_addr_pml4e_child = mem_allocate();
            if(page_addr_pml4e_child){
                pml4e_child[i]=((page_addr_pml4e_child) | PAGE_PERM);
            }
            
            /* Level 3 */
            pdpe_t = (pml4e[i] + KERN_MEM);
            pdpe = (uint64_t *)(pdpe_t & (0xFFFFFFFFFFFFF000));
            pdpe_t = (pml4e_child[i] + KERN_MEM);
            pdpe_child = (uint64_t *)(pdpe_t & (0xFFFFFFFFFFFFF000));
            for (int j=0; j<512; j++){
                if(pdpe[j] & PAGE_PRESENT){
                    page_addr_pdpe_child = mem_allocate();
                    if(page_addr_pdpe_child){
                        pdpe_child[j]=((page_addr_pdpe_child) | PAGE_PERM);
                    }
                    
                    /* Level 2 */
                    pde_t = (pdpe[j] + KERN_MEM);
                    pde = (uint64_t *)(pde_t & (0xFFFFFFFFFFFFF000));
                    pde_t = (pdpe_child[j] + KERN_MEM);
                    pde_child = (uint64_t *)(pde_t & (0xFFFFFFFFFFFFF000));
                    for (int k=0; k<512; k++){
                        if(pde[k] & PAGE_PRESENT){
                            page_addr_pde_child = mem_allocate();
                            if(page_addr_pde_child){
                                pde_child[k]=((page_addr_pde_child) | PAGE_PERM);
                            }
                
                            /* Level 1 */
                            pte_t = (pde[k] + KERN_MEM);
                            pte = (uint64_t *)(pte_t & (0xFFFFFFFFFFFFF000));
                            pte_t = (pde_child[k] + KERN_MEM);
                            pte_child = (uint64_t *)(pte_t & (0xFFFFFFFFFFFFF000));
                            for (int l=0; l<512; l++){
                                if(pte[l] & PAGE_PRESENT){
                                    pte[l]=(pte[l] & ~PAGE_WRITE);
                                    pte_child[l]=pte[l];
                                    
                                    pte_t = (pte_child[l] & (0xFFFFFFFFFFFFF000));
                                    struct memory_map *memmap_ent = get_physical_memmap(pte_t);
                                    if(!memmap_ent)
                                        return -1;
                                    memmap_ent->ref_cnt++;
                                    memmap_ent->cow_flag=true;
                                }
                            }
                        }
                    }
                }
            }
        }
    }

	return 0;
}


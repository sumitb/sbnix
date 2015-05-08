#include <sys/memory.h>

void init_memmap(void *physfree){
	long size=MAX_MEM;
	//uint64_t addr_t=INITIAL_MEM;
	uint64_t addr_t=0;
	//uint64_t addr_t=(uint64_t)physbase;
	long i=0;
	for(i=0; i<size; i++){
		if(addr_t<(uint64_t)physfree){
			memmap[i].ref_cnt=1;
			memmap[i].res_flag=BUSY;
		}
		else{
			memmap[i].ref_cnt=0;
			memmap[i].res_flag=AVAIL;
		}
        memmap[i].addr=addr_t;
        memmap[i].cow_flag=false;
		addr_t=addr_t+4096;
	}
//	printk("Memory Initialized");
}

uint64_t mem_allocate(){
	long i=0;
	for(i=0; i<MAX_MEM; i++){
		if(memmap[i].res_flag==AVAIL){
			memmap[i].ref_cnt++;
			memmap[i].res_flag=BUSY;
			return memmap[i].addr;
		}
	}
	return 0;
}

/* Do not memset page to 0, as it will affect performance
 * kernel space is already trusted just rewite on it
 */
uint64_t kmalloc(uint64_t size){
	if(size>0){
		int cnt=0;
		int i=0;
		cnt = size/4096;
		if(size%4096>0)
			cnt++;
		uint64_t ret_addr;
		ret_addr=(uint64_t)(KERN_MEM + mem_allocate());
		for(i=1;i<cnt;i++){
			mem_allocate();
		}
		return ret_addr;
	}
	return 0;
}

void  mem_free(uint64_t addr_t){
	long pg_no=(addr_t-INITIAL_MEM)/4096;
	if(memmap[pg_no].addr==addr_t){
		memmap[pg_no].res_flag=AVAIL;
	}
	else{
		long i=0;
		for(i=0; i<MAX_MEM; i++){
			if(memmap[i].addr==addr_t){
				memmap[i].res_flag=AVAIL;
			}
		}
	}
}

uint64_t page_roundoff_4096(uint64_t addr){
	if(addr%4096==0)
		return addr;
	else{
		uint64_t new_pg_addr= addr + (4096-(addr%4096));
		if(new_pg_addr>=END_LIMIT)
			return 0;
		else
			return new_pg_addr;
	}
}

uint64_t page_roundless_4096(uint64_t addr){
	if(addr%4096==0)
		return addr;
	else{
		uint64_t new_pg_addr= addr - (addr%4096);
		if(new_pg_addr<0)
			return 0;
		else
			return new_pg_addr;
	}
}

uint64_t stack_roundless(uint64_t addr){
	if(addr%8==0)
		return addr;
	else{
		uint64_t new_pg_addr= addr - (addr%8);
		if(new_pg_addr<0)
			return 0;
		else
			return new_pg_addr;
	}
}

uint64_t addr_res(uint64_t logical, int flag){
	if (flag == PML4E)
		return ((logical>>39) & 0x1FF);
	else if (flag == PDPE)
		return ((logical>>30) & 0x1FF);
	else if (flag == PDE)
		return ((logical>>21) & 0x1FF);
	else if (flag == PTE)
		return ((logical>>12) & 0x1FF);
	return 0;
}

struct memory_map *get_physical_memmap(uint64_t physical){
    physical = (physical & (0xFFFFFFFFFFFFF000));
    for(int i=0; i<MAX_MEM; i++){
        if(memmap[i].addr == physical){
            return &memmap[i];
        }
    }
    return NULL;
}

uint64_t* look_pages(uint64_t *pml4e,uint64_t logical){
	uint64_t *pdpe, *pde, *pte;
	uint64_t pdpe_t, pde_t, pte_t;
		
	if(pml4e[addr_res(logical,PML4E)] & PAGE_PRESENT){
        pdpe_t = (pml4e[addr_res(logical,PML4E)] + KERN_MEM);
        pdpe = (uint64_t *)(pdpe_t & (0xFFFFFFFFFFFFF000));

        if(pdpe[addr_res(logical,PDPE)]){
            pde_t = (pdpe[addr_res(logical,PDPE)] + KERN_MEM);
            pde = (uint64_t *)(pde_t & (0xFFFFFFFFFFFFF000));
            
            if(pde[addr_res(logical,PDE)]){
                pte_t = (pde[addr_res(logical,PDE)] + KERN_MEM);
                pte = (uint64_t *)(pte_t & (0xFFFFFFFFFFFFF000));

                if(pte[addr_res(logical,PTE)]){
                    return &pte[addr_res(logical,PTE)];
                }
            }
        }
	}
    return NULL;
}

uint64_t* walk_pages(uint64_t *pml4e,uint64_t logical){
	uint64_t *pdpe;
	uint64_t pdpe_t;
	uint64_t *pde;
	uint64_t pde_t;
	uint64_t *pte;
	uint64_t pte_t;
	uint64_t page_addr_pml4e;
	uint64_t page_addr_pdpe;
	uint64_t page_addr_pde;
		
	if(!pml4e[addr_res(logical,PML4E)] & PAGE_PRESENT){
		page_addr_pml4e = mem_allocate();
		if(page_addr_pml4e){
			//TODO:set page_present,page_write and page_user
			pml4e[addr_res(logical,PML4E)]=((page_addr_pml4e) | PAGE_PERM);
		}
	}
	pdpe_t = (pml4e[addr_res(logical,PML4E)] + KERN_MEM); 
	pdpe = (uint64_t *)(pdpe_t & (0xFFFFFFFFFFFFF000));

	if(!pdpe[addr_res(logical,PDPE)]){
		page_addr_pdpe = mem_allocate();
		if(page_addr_pdpe){
			//TODO:set page_present,page_write and page_user
			pdpe[addr_res(logical,PDPE)]=((page_addr_pdpe) | PAGE_PERM);
		}
	}
	pde_t = (pdpe[addr_res(logical,PDPE)] + KERN_MEM);
	pde = (uint64_t *)(pde_t & (0xFFFFFFFFFFFFF000));
	
	if(!pde[addr_res(logical,PDE)]){
		page_addr_pde = mem_allocate();
		if(page_addr_pde){
			//TODO:set page_present,page_write and page_user
			pde[addr_res(logical,PDE)]=((page_addr_pde) | PAGE_PERM);
		}
	}
	pte_t = (pde[addr_res(logical,PDE)] + KERN_MEM);
	pte = (uint64_t *)(pte_t & (0xFFFFFFFFFFFFF000));
	pte = &(pte[addr_res(logical,PTE)]);
	
	return pte;
}

void map_kernel(uint64_t *pml4e, uint64_t logical, uint64_t phys, uint64_t sz){
	uint64_t *pte;
	uint64_t vir_addr;
	uint64_t physical;
	uint64_t size;
	
	vir_addr = page_roundless_4096(logical);
	physical = page_roundless_4096(phys);
	size = page_roundoff_4096(sz);
	
	if(vir_addr){
		int i=0,j=0;
		for(i=0; i<size; i+=4096,j++){
			pte=walk_pages(pml4e,vir_addr+i);
			//TODO:set page_present,page_write and page_user
			*pte=((physical+i) | PAGE_PERM);
		}
	}
	
}

void mem_init(void* physbase,void *physfree){
	init_memmap(physfree);
	pml4e=(uint64_t *)mem_allocate();
	memset(pml4e,0,4096);
	cr3_addr=(uint64_t)pml4e;
	
	//map free memory from [100000-7FFE000]
	//map_kernel(pml4e,KERN_MEM+INITIAL_MEM,INITIAL_MEM,END_LIMIT-INITIAL_MEM);
	map_kernel(pml4e,KERN_MEM+(uint64_t)physbase,(uint64_t)physbase,END_LIMIT-(uint64_t)physbase);
	//map_video_memory
	map_kernel(pml4e,KERN_MEM+VIDEO_MEM,VIDEO_MEM,8192);
	pml4e=(uint64_t *)(KERN_MEM+cr3_addr);
	__asm__ __volatile__("mov %0, %%cr3":: "g"(cr3_addr));
}

void page_insert(uint64_t *pml4e, uint64_t logical, uint64_t physical){
	uint64_t *pte;
	pte=walk_pages(pml4e,logical);
	if(*pte & PAGE_PRESENT)
		mem_free(*pte);
	*pte=(physical | PAGE_PERM);
}

void kmalloc_user_space(uint64_t *pml4e,uint64_t logical, uint64_t size){
	int cnt=0;
	int i=0;
	cnt = size/4096;
	if(size%4096>0)
		cnt++;
	uint64_t p_addr=0;
	uint64_t addr=0;
	for(i=0;i<cnt;i++,addr+=4096){
		p_addr=(uint64_t)(mem_allocate());
		page_insert(pml4e,logical+addr,p_addr);
	}
}

void allocate(uint64_t pml4e_addr,void * addr, int len){
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
            OOM();
        else{
            page_insert((uint64_t *)pml4e_addr,start+cnt, p);
        }
    }
}


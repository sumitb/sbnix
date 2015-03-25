#include <sys/sbunix.h>
#include <sys/memory.h>

uint64_t *pml4e;
uint64_t cr3_addr;
void* memset(void *str,int val,uint64_t size){
    unsigned char* ptr = str;
    while(size--)
        *ptr++ = (unsigned char) val;
    return str;
}

void init_memmap(void *physfree){
	long size=MAX_MEM;
	//uint64_t addr_t=INITIAL_MEM;
	uint64_t addr_t=0;
	//uint64_t addr_t=(uint64_t)physbase;
	long i=0;
	for(i=0; i<size; i++){
		if(addr_t<(uint64_t)physfree){
			memmap[i].addr=addr_t;
			memmap[i].res_flag=BUSY;
		}
		else{
			memmap[i].addr=addr_t;
			memmap[i].res_flag=AVAIL;
		}
		addr_t=addr_t+4096;
	}
//	printk("Memory Initialized");
}

uint64_t mem_allocate(){
	long i=0;
	for(i=0; i<MAX_MEM; i++){
		if(memmap[i].res_flag==AVAIL){
			memmap[i].res_flag=BUSY;
			return memmap[i].addr;
		}
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
uint64_t page_roundoff(uint64_t addr){
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

uint64_t pml4e_shift(uint64_t logical){
	return ((logical>>39) & 0x1FF);
}

uint64_t pdpe_shift(uint64_t logical){
	return ((logical>>30) & 0x1FF);
}

uint64_t pde_shift(uint64_t logical){
	return ((logical>>21) & 0x1FF);
}

uint64_t pte_shift(uint64_t logical){
	return ((logical>>12) & 0x1FF);
}

uint64_t* walk_pde(uint64_t *pde,uint64_t logical){
	uint64_t *pte;
	uint64_t page_addr;
		
	if(!pde[pde_shift(logical)]){
		page_addr = mem_allocate();
		if(page_addr){
			//TODO:set page_present,page_write and page_user
			pde[pde_shift(logical)]=((page_addr) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
		}
	}
	pte = (uint64_t *)(pde[pde_shift(logical)] & ~(0xFFF)) + KERN_MEM;	
	pte = &(pte[pte_shift(logical)]);
	return pte;
}

uint64_t* walk_pdpe(uint64_t *pdpe,uint64_t logical){
	uint64_t *pde;
	uint64_t *pte;
	uint64_t page_addr;
		
	if(!pdpe[pdpe_shift(logical)]){
		page_addr = mem_allocate();
		if(page_addr){
			//TODO:set page_present,page_write and page_user
			pdpe[pdpe_shift(logical)]=((page_addr) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
		}
	}
	pde = (uint64_t *)(pdpe[pdpe_shift(logical)] & ~(0xFFF)) + KERN_MEM;	
	pte = walk_pde(pde, logical);
	return pte;
}

uint64_t* walk_pml4e(uint64_t *pml4e,uint64_t logical){
	uint64_t *pdpe;
	uint64_t *pte;
	uint64_t page_addr;
		
	if(!pml4e[pml4e_shift(logical)] & PAGE_PRESENT){
		page_addr = mem_allocate();
		if(page_addr){
			//TODO:set page_present,page_write and page_user
			pml4e[pml4e_shift(logical)]=((page_addr) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
		}
	}
	//pdpe = (uint64_t *)page_addr + KERN_MEM;	
	pdpe = (uint64_t *)(pml4e[pml4e_shift(logical)] & ~(0xFFF)) + KERN_MEM;	
	pte = walk_pdpe(pdpe, logical);
	return pte;
}

void map_kernel(uint64_t *pml4e, uint64_t logical, uint64_t physical, uint64_t size){
	uint64_t *pte;
	uint64_t vir_addr = page_roundoff(logical);
	size = page_roundoff(size);
	physical = page_roundoff(physical);
	if(vir_addr){
		int i=0,j=0;
		for(i=0; i<size; i+=4096,j++){
			pte=walk_pml4e(pml4e,vir_addr+i);	
			//TODO:set page_present,page_write and page_user
			*pte=((physical+i) | PAGE_PRESENT | PAGE_WRITE | PAGE_USER);
		}
	}
	
}

void mem_init(void* physbase,void *physfree){
	pml4e=(uint64_t *)mem_allocate();
	memset(pml4e,0,4096);
	cr3_addr=(uint64_t)pml4e;
	init_memmap(physfree);
	
	//map free memory from [100000-7FFE000]
	//map_kernel(pml4e,KERN_MEM+INITIAL_MEM,INITIAL_MEM,END_LIMIT-INITIAL_MEM);
	map_kernel(pml4e,KERN_MEM+(uint64_t)physbase,(uint64_t)physbase,END_LIMIT-(uint64_t)physbase);
	//map_video_memory
	map_kernel(pml4e,KERN_MEM+VIDEO_MEM,VIDEO_MEM,8192);
	
	__asm__ __volatile__("mov %0, %%cr3":: "b"(cr3_addr));
	
	int a =1;
	a++;
	 *((char*)KERN_MEM+0xb8000)=66;
	 *((char*)KERN_MEM+0xb8001)=0x07;
	
}

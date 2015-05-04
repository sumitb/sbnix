#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

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

/* Do not memset page to 0, as it will affect performance
 * kernel space is already trusted just rewite on it
 */
uint64_t kmalloc(uint64_t size){
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
	__asm__ __volatile__("mov %0, %%cr3":: "b"(cr3_addr));
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

void page_fault(){
	uint64_t *pte;
	uint64_t *pml4e_addr;
	uint64_t logical; 
	uint64_t physical;
	physical=mem_allocate();
	__asm__ __volatile__ ("movq %%cr2, %0" : "=r"(logical));
	pml4e_addr=(uint64_t *)currentTask->pml4e_addr;
	pte=walk_pages(pml4e_addr,logical);
	if(*pte & PAGE_PRESENT)
		mem_free(*pte);
	*pte=(physical | PAGE_PERM);
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
            printk("out of memory\n");
        else{
                page_insert((uint64_t *)pml4e_addr,start+cnt, p);
        }
    }
}

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

void protection_fault() {
	printk("protection fault\n");
}

uint64_t sys_brk(uint64_t bump_addr){
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


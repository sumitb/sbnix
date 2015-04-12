#include <sys/console.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <string.h>

void tarfs_initialize(){
	struct posix_header_ustar *tarfs_str;
	tarfs_str=(struct posix_header_ustar *)&_binary_tarfs_start;
	int ind=0;
	printk("tarfs_struct\n");
	while(tarfs_str < (struct posix_header_ustar *)&_binary_tarfs_end){
		strcpy(tarfs_ind[ind].name,tarfs_str->name);
		printk("file_name : %s\n",tarfs_str->name);
	 	tarfs_ind[ind].file_size=atoo(tarfs_str->size);
		tarfs_ind[ind].start_addr=tarfs_str++;
		if( (tarfs_ind[ind].file_size%512)==0)
			tarfs_str += (tarfs_ind[ind].file_size/512);
		else
			tarfs_str += (tarfs_ind[ind].file_size/512)+1;
		ind++;
	}
}


static void elf_load(){

    elf_header *elf=(elf_header *)(&);
    int len = elf->e_phnum;
    pheader *ph1;
    pheader *ph2;
   
    for(int i=0;i<len;i++){
        ph1 = (pheader *)(elf + (elf->e_phoff +(elf->e_phentsize * i)));
        if(ph1->p_type==ELF_PROG_LOAD){
            allocate(t,(void *) ph1->p_vaddr, ph1->p_memsz);
            lcr3(t->cr3);
            memset((char*) ph1->p_vaddr,0,ph1->p_memsz);
            
            memcpy((char *) ph1->p_vaddr, (void *) elf + ph1->p_offset, ph1->p_filesz);
            lcr3(boot_cr3);        
            struct vm_struct* vm = allocate_vma(t->mm);
            vm->vm_start = ph1->p_vaddr;
            vm->vm_end = ph1->p_vaddr + ph1->p_memsz;
            vm->vm_mmsz=ph1->p_memsz;
            vm->vm_next=NULL;
            vm->vm_file=(uint64_t)elf;

        
        }

    
    }


t->entry=elf->e_entry;
t->heap_vma = (struct vm_area_struct *)(KERN_MEM+mem_allocate());
struct vm_area_struct* tmp = t->mm->mmap;
while(t->)




}


static void allocate(t,void * addr, int len){

    int start = ROUNDDOWN((int)addr, PGSIZE);
    int end = ROUNDUP((int)addr+len, PGSIZE);
    page* p;
    
    for(int i=start;i<end;i+=PGSIZE){
        if(!(p=page_alloc(ALLOC_ZERO)))
            kprintf("out of memory\n");
        else{
            if(page_insert....)
                kprint("out of memory\n");

        
        }
    
    
    }


}



struct vma* allocate_vma(struct mm_struct* mm){
    char *node;
    struct vma* end;
    if(mm->mmap == NULL){
            node=(char *)(kernbase+mem_allocate());
            end=(struct vma *)node;
            mm->mmap=end;
            //mm->counti+=1;
            return (struct vma*)node;

        }
    else{
        end=mm->mmap;
        while(end->vm_next!=NULL)
            end=end->vm_next;
        node=sizeof(struct vma)+(char *)end;
        end->vm_next = (struct vma *)node;
        return (struct vma *)node;
    
    }

}

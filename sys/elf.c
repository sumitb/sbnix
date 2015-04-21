#include <sys/elf.h>
#include <sys/tarfs.h>
#include <sys/sched.h>
#include <sys/memory.h>
#include <sys/console.h>

void elf_load(struct task *t, char *file_name){
	uint64_t offset = check_file(file_name);
	if(offset!=0){
		elf_header *elf = (elf_header *)offset;
	    pheader *ph1=(pheader *)((uint8_t *)elf + elf->e_phoff);
		int ph_cnt=elf->e_phnum;

		for(int i=0;i<ph_cnt;i++,ph1++){
			if(ph1->p_type==1){
				allocate(t->pml4e_addr,(void *) ph1->p_vaddr, ph1->p_memsz);
				__asm __volatile("movq %0,%%cr3" : : "r" (t->cr3_address));
				memset((char*) ph1->p_vaddr,0,ph1->p_memsz);
				memcpy((char *) ph1->p_vaddr, (void *) elf + ph1->p_offset, ph1->p_filesz);
				__asm __volatile("movq %0,%%cr3" : : "r" (cr3_addr));       
				vma *vm = allocate_vma(t->mm->vma_addr);
				vm->vm_start = ph1->p_vaddr;
				vm->vm_end = ph1->p_vaddr + ph1->p_memsz;
				vm->vm_mmsz=ph1->p_memsz;
				vm->vm_next=NULL;
				vm->vm_file=(uint64_t)elf;
				vm->vm_offset=ph1->p_offset;
				//start=vm->vm_start;
				//end=vm->vm_end;
			}
		}

		t->entry_pt=elf->e_entry;
		/*t->heap_vma = (vma *)(KERN_MEM+mem_allocate());
		vma* tmp = t->mm->mmap;
		while(tmp->vm_next != NULL)
			tmp=tmp->vm_next;

		 uint64_t val = ALIGN_DOWN((uint64_t)(tmp->vm_end + 0x1000));
		 t->heap_vma->vm_end=t->heap_vma->vm_start=val;
		 t->heap_vma->vm_mmsz=0x1000;

		allocate(t,(void *)t->heap_vma->vm_start,t->heap_vma->vm_mmsz);
		t->mm->mmap->vm_start=start;
		t->mm->mmap->vm_end=end;*/
	}

}


/*
#include <sys/process.h>
#include<sys/tarfs.h>
#include<sys/memory.h>
#include<sys/console.h>
#include<sys/defs.h>


int brk(void *end_data_segment){
    vma* temp= running_proc.mm->mmap;
    while(temp->vm_next!=NULL )
        temp=temp->vm_next;
    uint64_t start=temp->vm_start;
    uint64_t end=temp->vm_end;
    uint64_t used= (end-start)%4096;
    uint64_t left=4096-used;
    if(left>(end_data_segment-end)){
        temp->vm_end=end_data_segment;
        return 0;
    }
    
    else{
      uint64_t size= (end_data_segment-end)-left;
      uint64_t pgstart=end+left;
      allocate(running_proc,pgstart,size);
      return 0;
    
    }

}


void free(void *ptr){
    vma* temp= running_proc.mm->mmap;
    while(temp->vm_next!=NULL )
        temp=temp->vm_next;
    uint64_t start=temp->vm_start;
    uint64_t end=temp->vm_end;
    temp->vm_end=ptr;

}


void *malloc(size_t size){
    vma* temp= running_proc.mm->mmap;
    while(temp->vm_next!=NULL )
        temp=temp->vm_next;
    uint64_t start=temp->vm_start;
    uint64_t end=temp->vm_end;
   int val= brk(end+size);
   if(val==0)
       return (void *)end;
   else
       return NULL;
}

unsigned int sleep(unsigned int seconds){

    struct task process = running_proc->process;
    

}

unsigned int alarm(unsigned int seconds){


}


int chdir(const char *path){

}

char *getcwd(char *buf, size_t size){


}
pid_t getpid(){

    return (running_proc.process.process_id);
}

pid_t getppid(){

    return(running_proc.process.parent_id);

}
*/

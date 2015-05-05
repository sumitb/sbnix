#include <sys/console.h>
#include <sys/tarfs.h>
#include <sys/memory.h>
#include <sys/sched.h>



int sys_dup2(int old_fd, int new_fd){
	struct task_struct *process = getCurrentTask();
	if(old_fd > process->fd_cnt || old_fd < 0){
		return -1;
	}
	if(new_fd > process->fd_cnt || new_fd < 0){
		return -1;
	}
	process->dup_arr[old_fd]=new_fd;
	return new_fd;
}

int sys_pipe(int fd[]){
	struct task_struct *process = getCurrentTask();
	char *pipe_buff = (char *)kmalloc(4096);
	if(pipe_buff){
		process->fd_cnt++;
		int fd1=process->fd_cnt++;
		int fd2=process->fd_cnt;
		
		fd[0]=fd1;
		fd[1]=fd2;
		
		process->fd[fd1].offset=(uint64_t)pipe_buff;
		strcpy(process->fd[fd1].path,"pipe_buffer");
		
		process->fd[fd2].offset=(uint64_t)pipe_buff;
		strcpy(process->fd[fd2].path,"pipe_buffer");
		return 0;
	}
	return 1;
}

#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <asm/errno.h>

asmlinkage long sys_set_myFlag(pid_t pid, int flag){
	
	if(!capable(CAP_SYS_ADMIN)){  // Root check
		printk(KERN_ALERT "SYS_MYFLAG: No root user\n");
		return -EPERM;
	}
	
	if(flag != 0 && flag != 1){ // Wrong Flag value
		printk(KERN_ALERT "SYS_MYFLAG: Wrong flag value 0 or 1\n");
		return -EINVAL;
	}
	
	struct task_struct *process = find_task_by_vpid(pid);
	
	if(process == NULL){ // No such process found
		printk(KERN_ALERT "SYS_MYFLAG: No such process found");
		return -ESRCH;
	}
		
	process->myFlag = flag;

	return 0;
}

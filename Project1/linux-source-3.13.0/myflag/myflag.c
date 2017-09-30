#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>
#include <asm/errno.h>

asmlinkage long sys_set_myFlag(pid_t pid, int flag){
	
	if(!capable(CAP_SYS_ADMIN)){  // Root program check
		printk(KERN_ALERT "DEBUG: No root user\n");
		return -EPERM;
	}
	
	// printk(KERN_ALERT "DEBUG: eperm: %d einval: %d esrch: %d\n", EPERM, EINVAL, ESRCH);
	
	printk(KERN_ALERT "DEBUG: Flag check\n");
	if(flag != 0 && flag != 1){ // Wrong Flag value
		printk(KERN_ALERT "DEBUG: Wrong flag value 0 or 1\n");
		return -EINVAL;
	}
	
	struct task_struct *process = find_task_by_vpid(pid);
	
	printk(KERN_ALERT "DEBUG: Process check\n");
	if(process == NULL){ // No such process found
		printk(KERN_ALERT "DEBUG: No such process found");
		return -ESRCH;
	}
	
	printk(KERN_ALERT "DEBUG: Process got!\n");
	printk(KERN_ALERT "DEBUG: Process current flag %d\n", process->myFlag);
	
	process->myFlag = flag;
	
	printk(KERN_ALERT "DEBUG: Process flag %d\n", process->myFlag);
	
	return 0;
}

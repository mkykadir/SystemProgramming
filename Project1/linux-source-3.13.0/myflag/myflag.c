#include <linux/syscalls.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/pid.h>

asmlinkage long sys_set_myFlag(pid_t pid, int flag){
	struct task_struct *process = find_task_by_vpid(pid);
	printk(KERN_ALERT "Process got!\n");
	process->myFlag = flag;
	printk(KERN_ALERT "Process flag %d\n", process->myFlag);
	return 0;
}

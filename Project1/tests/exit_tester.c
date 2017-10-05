/*
 * 2017, İTÜ. System Programming Course
 * 
 * exit_tester.c - exit System Call Test Program
 * 
 * This program forks two children processes and then waits for a key
 * input to close parent program. This program waits flag value as
 * console argument from user (0 or 1). Since system call to update flag
 * needs root privilleges, this program should be run as root. If you
 * send flag value as 1 and "renice" the parent process with a value 
 * that is bigger than 10 and press a key on keyboard, all children 
 * processes will be killed along with parent. 
 * 
 * sudo ./exit_tester <Flag = 0 or 1>
 * Example:
 * 	sudo ./exit_tester 1
 * 
 * System call needs root privilleges
 * System call will check if process exists in the system
 * System call will check for flag value, only 1 and 0 allowed
 * Check dmesg for errors
 * 
 * 150140119 - Muhammed Kadir Yücel
 * 150140123 - Mahmut Lutfullah Özbilen
 * 
 */

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#define NR_myflag 355

int main(int argc, char** argv)
{
	if(argc != 2){
		printf("Usage: %s <Flag = 0 or 1>\n", argv[0]);
		return -1;
	}
	
	int flagValue = atoi(argv[1]);
	
	pid_t parentPID = getpid();
	
	int y;
	y = syscall(NR_myflag, parentPID, flagValue); // set myFlag for parent
	if(y == -1){
		printf("Return Value: %d, ", errno);
		switch(errno){
			case EPERM:
				printf("Needs root permissions\n");
				break;
			case EINVAL:
				printf("Invalid flag value\n");
				break;
			case ESRCH:
				printf("No such process with given PID in system\n");
				break;
			default:
				break;
		}
	}else{
		printf("Return Value: %d. Test passed, flag updated!\n", y);
	}
	
    pid_t firstPID;
    firstPID = fork();

    if(firstPID >= 0) // fork was successful
    {
        if(firstPID == 0) // first child process
        {
            printf("First child ID: %d, parent ID: %d\n", getpid(), getppid());
        }
        else //Parent process
        {
            pid_t secondPID;
			secondPID = fork();
			if(secondPID >= 0){
				if(secondPID == 0){ // second child process
					printf("Second child ID: %d, parent ID: %d\n", getpid(), getppid());
				}
			}else{
				printf("Fork failed for second child!\n");
				return 1;
			}
        }
    }
    else // fork failed
    {
        printf("\n Fork failed, quitting!\n");
        return 1;
    }
    
    getchar();
	exit(0);

}

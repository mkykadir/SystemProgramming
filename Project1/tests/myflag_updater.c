/*
 * 2017, İTÜ. System Programming Course
 * 
 * myflag_updater.c - myFlag Value Updater Test Program
 * 
 * This program updates the "myFlag" value inside task_struct of each
 * process. PID of the process and flag value should be given as
 * console arguments. You need root privilleges to run this program.
 * 
 * sudo ./myflag_updater <PID> <Flag = 0 or 1>
 * Example:
 * 	sudo ./myflag_updater 1234 1
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

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#define NR_myflag 355

int main(int argc, char** argv){
	
	if(argc != 3){
		printf("Invalid console arguments: %s <PID> <FLAG= 0 or 1>\n", argv[0]);
		return -1;
	}
		
	int processid = atoi(argv[1]);
	int flagValue = atoi(argv[2]);
	
	int y;
	pid_t processPid = processid;
	y = syscall(NR_myflag, processPid, flagValue);
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
	
	exit(0);
}

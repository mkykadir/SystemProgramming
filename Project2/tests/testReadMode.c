/*
 * 2017, İTÜ. System Programming Course
 * 
 * testReadMode.c - Ioctl Test Program
 *  
 * 
 * 150140119 - Muhammed Kadir Yücel
 * 150140123 - Mahmut Lutfullah Özbilen
 * 
 */

#include <stdio.h>
#include <stdlib.h> 
#include <sys/stat.h> 
#include <errno.h>
#include <fcntl.h>

#include "../driver_ioctl.h"

int main(int argc, char** argv){
	if(argc != 3){
		printf("Usage: Device and read mode: 0 or 1.\n");
		return -1;
	}
	
	int readModeValue = atoi(argv[2]);
	/*if(readModeValue != 1 || readModeValue != 0){
		printf("Read mode can not be other than 1 or 0.\n");
		return -1;
	}*/
	int fd = open(argv[1], O_RDONLY);
	if(fd == -1){
		printf("Cannot open file descripter.\n");
		return -1;
	}
	int ret;
	ret = ioctl(fd, SET_READ_MODE, readModeValue);
	if(ret == -1){
		printf("Return value: %d ", errno);
		switch(errno){
			case EPERM:
				printf("Needs root permission\n");
			default:
				break;
		}
	}
	else{
		printf("Return value: %d. Test passed, read mode changed!\n", ret);
	}
	close(fd);
	return 0;
}

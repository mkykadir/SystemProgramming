#include "driver_ioctl.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>

int main(int argc, char** argv){
	
	if(argc != 4){
		printf("Usage: ./ioctl device_file_name op_code arg\n");
		return -1;
	}
	
	int device_file = open(argv[1], O_RDONLY);
	if(device_file < 0){
		printf("Cannot open device file %s , check if exists!\n", argv[1]);
		return -1;
	}
	
	int ret;
	int op_code = atoi(argv[2]);
	int arg = atoi(argv[3]);
	
	// right now we are taking user id
	// if get username as char* we should convert it!
	
	ret = ioctl(device_file, op_code, arg);
	if(ret < 0){
		printf("Return value %d\n", errno);
		if(errno == EPERM)
			printf("Needs root permissions\n");
		else if(errno == EINVAL)
			printf("There is no such operation in device IOCTL!\n");
	}else
		printf("IOCTL command executed successfully, %d!\n", ret);
		
	close(device_file);
	return 0;
	
	
}

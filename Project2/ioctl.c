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
		return 1;
	}
	
	int device_file = open(argv[1], 0);
	if(device_file < 0){
		printf("Cannot open device file %s , check if exists!\n", argv[1]);
		return 1;
	}
	
	
}

/*
 * 2017, İTÜ. System Programming Course
 *  
 * Project 2
 * 
 * 150140119 - Muhammed Kadir Yücel
 * 150140123 - Mahmut Lutfullah Özbilen
 * 
 */


#include "driver_ioctl.h"

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>

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
	int ioctlSelection = atoi(argv[2]);
	char* argChar = argv[3];
	
	switch(op_code){
		case 0:
			op_code = SET_READ_MODE;
			break;
		case 1:
			op_code = SET_UNREAD_LIMIT;
			break;
		case 2:
			op_code = CLEAR_MESSAGE_OF;
			break;
		default:
			break;		
	}
	int arg;
	// right now we are taking user id
	// if get username as char* we should convert it!
	if(ioctlSelection == 2){

		arg = username_to_userid(argChar);

		if(arg == -1){
			printf("There is no such user: %s\n", argChar);
			return -1;
		}
	}
	else{
		arg = atoi(argChar);
	}
	
	
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

int username_to_userid(char* username){
	char buffer[4096];
	int length=0;
	while(username[length] != '\0'){
		length++;
	}
	
	FILE *fp;
	fp = fopen("/etc/passwd","r");
	fseek(fp,0,SEEK_SET);
	fread(buffer,4096,1,fp);
	fclose(fp);
	int i = 0;
	int found = 0;
	int index = 0;
	
	while(buffer[index]!= EOF && !found){
		for(i = 0; i < length; i++){
			found = 1;
			if(username[i] != buffer[index + i]){
					found = 0;
					break;
			}
		}
		if(!found){
			while(buffer[index] != '\n' && buffer[index] != EOF)
				index++;
			if(buffer[index] != EOF)
				index++;
		}
	}
	if(found){
		index = index + length + 3;
		int uid = 0;
		for(i = index; buffer[i] != ':'; i++){
			uid = 10 * uid + buffer[i] - '0';
		}
		return uid;
	}
	else{
		printf("user cannot found\n");
		return -1;
	}
}

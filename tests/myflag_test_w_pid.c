#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#define NR_myflag 355

int main(int argc, char** argv){
	if(argc != 3){
		printf("Invalid console arguments: %s pid flag\n", argv[0]);
		return -1;
	}
		
	int processid = atoi(argv[1]);
	int flagValue = atoi(argv[2]);
	
	long y;
	pid_t parent = processid;
	printf("Parent id: %d\n", parent);
	y = syscall(NR_myflag, parent, flagValue);
	printf("%ld\n", y);
	return 0;
}

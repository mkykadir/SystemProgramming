#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <csv.h>

char* path;

static const char* names_path = "/NAMES";
static const char* codes_path = "/CODES";

void csv_data_updater(){
	// thinking...
}


static int fuse_getattr(const char* path, struct stat *stbuf){
	int res = 0;
	
	printf("PATH is: %s\n", path); // DEBUG
	int delim_count = 0;
	int i;
	for(i = 0; i < strlen(path); i++){
		if(path[i] == '/'){
			delim_count++;
			printf("DELIM_COUNT is: %d\n", delim_count); // DEBUG
		}
	}
	
	memset(stbuf, 0, sizeof(struct stat));
	if(strcmp(path, "/") == 0){
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}
	else if(strcmp(path, names_path) == 0){
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}
	else if(strcmp(path, codes_path) == 0){
		stbuf->st_mode = S_IFDIR | 0755;
		stbuf->st_nlink = 2;
	}
	else if(delim_count == 2){ // /NAMES/Istanbul or /CODES/34
		if(strstr(path, names_path) != NULL){ // /NAMES/Istanbul
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
		}
		else if(strstr(path, codes_path) != NULL){ // /CODES/34
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
		}
		else{
			res = -ENOENT;
		}
	}
	else if(delim_count == 3){ // /NAMES/Istanbul/Sariyer or /CODES/34/34398.txt
		if(strstr(path, names_path) != NULL){ // /NAMES/Istanbul/Sariyer
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
		}
		else if(strstr(path, codes_path) != NULL){ // /CODES/34/34398.txt
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			// need to get length of txt file
		}
		else{
			res = -ENOENT;
		}
	}
	else if(delim_count == 4){ // /NAMES/Istanbul/Sariyer/Maslak.txt
		if(strstr(path, names_path) != NULL){ // /NAMES/Istanbul/Sariyer/Maslak.txt
			stbuf->st_mode = S_IFREG | 0444;
			stbuf->st_nlink = 1;
			// need to get length of txt file
		}
		else{
			res = -ENOENT;
		}
	}else{
		res = -ENOENT;
	}
	
	return res;
}

static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
							off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;
	
	printf("PATH is: %s\n", path); // DEBUG
	int delim_count = 0;
	int i;
	for(i = 0; i < strlen(path); i++){
		if(path[i] == '/'){
			delim_count++;
			printf("DELIM_COUNT is: %d\n", delim_count); // DEBUG
		}
	}
	
	if(strcmp(path, "/") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		filler(buf, names_path+1, NULL, 0);
		filler(buf, codes_path+1, NULL, 0);
	}else if(strcmp(path, names_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		filler(buf, "Istanbul", NULL, 0); // DEBUG
	}else if(strcmp(path, codes_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		filler(buf, "34", NULL, 0); // DEBUG
	}else if(delim_count == 2){ // /NAMES/Istanbul or /CODES/34
		if(strstr(path, names_path) != NULL){ // Should containt folders
			filler(buf, "Sariyer", NULL, 0); // DEBUG
		}
		else if(strstr(path, codes_path) != NULL){ // Should contain files
			filler(buf, "34398.txt", NULL, 0); // DEBUG
		}
		else{
			return -ENOENT;
		}
	}
	else if(delim_count == 3){ // /NAMES/Istanbul/Sariyer
		if(strstr(path, names_path) != NULL){ // Should contain files
			filler(buf, "Maslak.txt", NULL, 0); // DEBUG
		}
		else{
			return -ENOENT;
		}
	}else
		return -ENOENT;	
	
	return 0;
}

static int fuse_open(const char *path, struct fuse_file_info *fi)
{		
		
	if((fi->flags & 3) != O_RDONLY)
		return -EACCES;
		
	return 0;
}

static struct fuse_operations fuse_oper = {
	.getattr = fuse_getattr,
	.readdir = fuse_readdir,
	.open = fuse_open,
};

int main(int argc, char* argv[])
{
	return fuse_main(argc, argv, &fuse_oper, NULL);
}

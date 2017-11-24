#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

char* path;

static const char* names_path = "/NAMES";
static const char* codes_path = "/CODES";

static int fuse_getattr(const char* path, struct stat *stbuf){
	int res = 0;
	
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
	else{
		res = -ENOENT;
	}
	
	return res;
}

static int fuse_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
							off_t offset, struct fuse_file_info *fi)
{
	(void) offset;
	(void) fi;
	
	if(strcmp(path, "/") == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		filler(buf, names_path+1, NULL, 0);
		filler(buf, codes_path+1, NULL, 0);
	}else if(strcmp(path, names_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
	}else if(strcmp(path, codes_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
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

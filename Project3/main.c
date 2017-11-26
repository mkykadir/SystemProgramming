#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <csv.h>

char* path;

// START: CSV things
typedef struct csv_row {
	char* code;
	char* neighborhood;
	char* city;
	char* district;
	char* latitude;
	char* longitude;
	struct csv_row* next;
} crow;

typedef struct linked_list {
	crow* head;
} llist;

int which_column = 0;
llist my_list;

void cb1(void *s, size_t len, void* data){
	crow* current = ((llist*)data)->head;
	if(current == NULL){
		printf("Current is null\n");
		return;
	}
	switch(which_column){
		case 0: //code
			current->code = (char*) malloc(len+1);
			strcpy(current->code, (char*)s);
			break;
		case 1: //neighbor
			current->neighborhood = (char*) malloc(len+1);
			strcpy(current->neighborhood, (char*)s);
			break;
		case 2: //city
			current->city = (char*) malloc(len+1);
			strcpy(current->city, (char*)s);
			break;
		case 3: //district
			current->district = (char*) malloc(len+1);
			strcpy(current->district, (char*)s);
			break;
		case 4: //lat
			current->latitude = (char*) malloc(len+1);
			strcpy(current->latitude, (char*)s);
			break;
		case 5: //long
			current->longitude = (char*) malloc(len+1);
			strcpy(current->longitude, (char*)s);
			break;
		default:
			break;
	}
	which_column++;
}

void cb2(int c, void *data){
	which_column = 0;
	crow* new_row = (crow*) malloc(sizeof(crow));
	new_row->next = ((llist*)data)->head;
	((llist*)data)->head = new_row;
}


static int is_space(unsigned char c) {
	if (c == CSV_SPACE || c == CSV_TAB) return 1;
	return 0;
}

static int is_term(unsigned char c) {
  if (c == CSV_CR || c == CSV_LF) return 1;
  return 0;
}

// END: CSV things

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
		crow* temp = my_list.head;
		while(temp != NULL){
			if(temp->city == NULL){
				temp = temp->next;
				continue;
			}
			
			crow* temp2 = my_list.head;
			while(temp2 != NULL && temp2!=temp){
				if(temp2->city == NULL){
					temp2 = temp2->next;
					continue;
				}
				if(strcmp(temp->city, temp2->city) == 0)
					break;
					
				temp2 = temp2->next;
			}
			if(temp == temp2)
				filler(buf, temp->city, NULL, 0);
				
			temp = temp->next;
		}
	}else if(strcmp(path, codes_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		filler(buf, "34", NULL, 0); // DEBUG
	}else if(delim_count == 2){ // /NAMES/Istanbul or /CODES/34
		if(strstr(path, names_path) != NULL){ // Should containt folders
			size_t city_name_size = strlen(path) - strlen(names_path);
			char* city_name = (char*) malloc(city_name_size);
			strcpy(city_name, path + 7);
			city_name[city_name_size - 1] = '\0';
			crow* temp = my_list.head;
			while(temp != NULL){
				if(temp->city == NULL){
					temp = temp->next;
					continue;
				}
				
				crow* temp2 = my_list.head;
				while(temp2 != NULL && temp2!=temp){
					if(temp2->city == NULL){
						temp2 = temp2->next;
						continue;
					}
					if(strcmp(temp->district, temp2->district) == 0)
						break;
						
					temp2 = temp2->next;
				}
				
				if(temp==temp2 && strcmp(temp->city, city_name) == 0){
					filler(buf, temp->district, NULL, 0);
				}
				temp = temp->next;
			}
			free(city_name);
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
			size_t double_name_size = strlen(path) - strlen(names_path);
			char* double_name = (char*) malloc(double_name_size);
			strcpy(double_name, path + 7);
			double_name[double_name_size - 1] = '\0'; // Istanbul/Sariyer
			
			printf("double_name is %s\n", double_name);
			
			int iterator;
			for(iterator = 0; iterator < double_name_size; iterator++){
				if(double_name[iterator] == '/'){
					double_name[iterator] = '\0';
					break;
				}
			}
			if (iterator == double_name_size)
				return -ENOENT;
			
			size_t city_name_size = strlen(double_name);
			char* city_name = (char*) malloc(city_name_size+1);
			strcpy(city_name, double_name);
			city_name[city_name_size] = '\0';
			
			printf("city_name is %s\n", city_name);
			
			size_t district_name_size = strlen(double_name + iterator +1);
			char* district_name = (char*) malloc(district_name_size+1);
			strcpy(district_name, double_name + iterator + 1);
			district_name[district_name_size] = '\0';
			printf("district_name is %s\n", district_name);
			
			crow *temp = my_list.head;
			
			while(temp != NULL){
				if(temp->city == NULL){
					temp = temp->next;
					continue;
				}
				
				crow* temp2 = my_list.head;
				while(temp2 != NULL && temp2!=temp){
					if(temp2->city == NULL){
						temp2 = temp2->next;
						continue;
					}
					if(strcmp(temp->neighborhood, temp2->neighborhood) == 0)
						break;
						
					temp2 = temp2->next;
				}
				
				if(temp==temp2 && strcmp(temp->city, city_name) == 0 && strcmp(temp->district, district_name) == 0){
					filler(buf, temp->neighborhood, NULL, 0);
				}
				temp = temp->next;
			}
			free(district_name);
			free(city_name);
			free(double_name);
			
			
			
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

static int fuse_read(const char *path, char *buf, size_t size, off_t offset,
						struct fuse_file_info *fi)
{
	size_t len;
	(void) fi;
	
	int delim_count = 0;
	int i;
	for(i = 0; i < strlen(path); i++){
		if(path[i] == '/'){
			delim_count++;
			printf("DELIM_COUNT is: %d\n", delim_count); // DEBUG
		}
	}
}

static struct fuse_operations fuse_oper = {
	.getattr = fuse_getattr,
	.readdir = fuse_readdir,
	.open = fuse_open,
	.read = fuse_read,
};

int main(int argc, char* argv[])
{	
	FILE* fp;
	struct csv_parser p;
	char buf[1024];
	size_t bytes_read;
	unsigned char options = CSV_STRICT | CSV_APPEND_NULL;
	
	crow* first = (crow*)malloc(sizeof(crow));
	my_list.head = first;
	
	if(csv_init(&p, options) != 0){
		printf("Failed to initialize csv parser\n");
		exit(EXIT_FAILURE);
	}
	
	csv_set_space_func(&p, is_space);
	csv_set_term_func(&p, is_term);
	csv_set_delim(&p, '\t');
	fp = fopen("postal-codes.csv", "rb");
	if(!fp){
		printf("Failed to open file %s - %s\n", argv[1], strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	while((bytes_read = fread(buf, 1, 1024, fp)) > 0){
		size_t ret = csv_parse(&p, buf, bytes_read, cb1, cb2, &my_list);
		if(ret != bytes_read){
			printf("Error while parsing file :%s\n", csv_strerror(csv_error(&p)));
		}
	}
	
	csv_fini(&p, cb1, cb2, &my_list);
	
	fclose(fp);
	csv_free(&p);
	
	return fuse_main(argc, argv, &fuse_oper, NULL);
}

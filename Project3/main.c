#define FUSE_USE_VERSION 26

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <csv.h>

char* path;

static const char* fcode = "code: ";
static const char* fneighborhood = "\nneighborhood: ";
static const char* fcity = "\ncity: ";
static const char* fdistrict = "\ndistrict: ";
static const char* flatitude = "\nlatitude: ";
static const char* flongitude = "\nlongitude: ";

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

typedef struct is_added_list {
	crow* head;
} islist;

/*typedef struct slotes {
	char* sTring;
	struct slotes* next;
} slot;*/

/*typedef struct storage_list {
	slot* head;
} s_list;*/

int which_column = 0;
llist my_list;
//s_list trunk;

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

			char* token_path = (char*)malloc(strlen(path)+1);
			strcpy(token_path, path);
			char* token = strtok(token_path, "/");
			char* temp_code_name = (char*) malloc(3);

			char* plate;
			
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 2)
					plate = token;
				token = strtok(NULL, "/");
			}
			size_t file_length = 0;
			crow *temp = my_list.head;
			while(temp != NULL){
				if(temp->code == NULL){
					temp = temp->next;
					continue;
				}
				for(i = 0; i < 2; i++)
					temp_code_name[i] = temp->code[i];
				temp_code_name[2] = '\0';
				if(strcmp(temp_code_name, plate) == 0 ){
						
						
					file_length += strlen(fcode) + strlen(fneighborhood) +
						strlen(fcity) + strlen(fdistrict) +
						strlen(flatitude) + strlen(flongitude) +
						strlen(temp->code) + strlen(temp->neighborhood) +
						strlen(temp->city) + strlen(temp->district) +
						strlen(temp->latitude) + strlen(temp->longitude) + 1;
					
				}
				temp = temp->next;
			}

			stbuf->st_size = file_length;
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
			
			char* token_path = (char*)malloc(strlen(path)+1);
			strcpy(token_path, path);
			char* token = strtok(token_path, "/");
			
			char* city;
			char* district;
			char* file_name;
			
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 2)
					city = token;
				if(i == 3)
					district = token;
				if(i == 4)
					file_name = token;
				token = strtok(NULL, "/");
			}
			
			char* neighborhood = strtok(file_name, ".");
			
			size_t file_length = 0;
			crow *temp = my_list.head;
			while(temp != NULL){
				if(temp->city == NULL){
					temp = temp->next;
					continue;
				}
				
				if(strcmp(temp->city, city) == 0 
					&& strcmp(temp->district, district) == 0
					&& strcmp(temp->neighborhood, neighborhood) == 0){
						
						
					file_length = strlen(fcode) + strlen(fneighborhood) +
						strlen(fcity) + strlen(fdistrict) +
						strlen(flatitude) + strlen(flongitude) +
						strlen(temp->code) + strlen(temp->neighborhood) +
						strlen(temp->city) + strlen(temp->district) +
						strlen(temp->latitude) + strlen(temp->longitude) + 1;
					break;
				}
				temp = temp->next;
			}

			stbuf->st_size = file_length;
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
		islist is_added;
		is_added.head = NULL;
		crow* temp = my_list.head;
		while(temp != NULL){
			if(temp->city == NULL){
				temp = temp->next;
				continue;
			}
			
			crow* is_temp = is_added.head;
			while(is_temp != NULL){
				if(is_temp->city == NULL){
					is_temp = is_temp->next;
					continue;
				}
				
				if(strcmp(temp->city, is_temp->city) == 0)
					break;
					
				is_temp = is_temp->next;
			}
			
			if(is_temp == NULL){
				crow* add_temp = (crow*)malloc(sizeof(crow));
				add_temp->code = (char*) malloc(strlen(temp->code)+1);
				strcpy(add_temp->code, temp->code);
				add_temp->neighborhood = (char*) malloc(strlen(temp->neighborhood)+1);
				strcpy(add_temp->neighborhood, temp->neighborhood);
				add_temp->city = (char*) malloc(strlen(temp->city)+1);
				strcpy(add_temp->city, temp->city);
				add_temp->district = (char*) malloc(strlen(temp->district)+1);
				strcpy(add_temp->district, temp->district);
				add_temp->latitude = (char*) malloc(strlen(temp->latitude)+1);
				strcpy(add_temp->latitude, temp->latitude);
				add_temp->longitude = (char*) malloc(strlen(temp->longitude)+1);
				strcpy(add_temp->longitude, temp->longitude);
				add_temp->next = is_added.head;
				is_added.head = add_temp;
				
				filler(buf, temp->city, NULL, 0);
			}
			temp = temp->next;
		}
	}else if(strcmp(path, codes_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		crow* temp = my_list.head;
		char* temp_code = (char*) malloc(3);
		char* temp_code2 = (char*) malloc(3);

		while(temp != NULL){
			if(temp->code == NULL){
				temp = temp->next;
				continue;
			}
			
			for(i = 0; i < 2; i++)
					temp_code[i] = temp->code[i];
			temp_code[2] = '\0';
			
			crow* temp2 = my_list.head;
			while(temp2 != NULL && temp2!=temp){
				if(temp2->code == NULL){
					temp2 = temp2->next;
					continue;
				}		
				for(i = 0; i < 2; i++)
						temp_code2[i] = temp2->code[i];
				temp_code2[2] = '\0';
				if(strcmp(temp_code, temp_code2) == 0)
					break;
					
				temp2 = temp2->next;
			}
			if(temp == temp2){
				
				filler(buf, temp_code, NULL, 0);
			}
			temp = temp->next;
		}
		free(temp_code);
		free(temp_code2);
	}else if(delim_count == 2){ // /NAMES/Istanbul or /CODES/34
		if(strstr(path, names_path) != NULL){ // Should containt folders
			size_t city_name_size = strlen(path) - strlen(names_path);
			char* city_name = (char*) malloc(city_name_size);
			strcpy(city_name, path + 7);
			city_name[city_name_size - 1] = '\0';
			islist is_added;
			is_added.head = NULL;
			crow* temp = my_list.head;
			while(temp != NULL){
				if(temp->city == NULL){
					temp = temp->next;
					continue;
				}
				
				crow* is_temp = is_added.head;
				while(is_temp != NULL){
					if(is_temp->city == NULL){
						is_temp = is_temp->next;
						continue;
					}
					
					if(strcmp(temp->city, is_temp->city) == 0 && strcmp(temp->district, is_temp->district) == 0)
						break;
						
					is_temp = is_temp->next;
				}
				
				if(is_temp == NULL){
					crow* add_temp = (crow*)malloc(sizeof(crow));
					add_temp->code = (char*) malloc(strlen(temp->code)+1);
					strcpy(add_temp->code, temp->code);
					add_temp->neighborhood = (char*) malloc(strlen(temp->neighborhood)+1);
					strcpy(add_temp->neighborhood, temp->neighborhood);
					add_temp->city = (char*) malloc(strlen(temp->city)+1);
					strcpy(add_temp->city, temp->city);
					add_temp->district = (char*) malloc(strlen(temp->district)+1);
					strcpy(add_temp->district, temp->district);
					add_temp->latitude = (char*) malloc(strlen(temp->latitude)+1);
					strcpy(add_temp->latitude, temp->latitude);
					add_temp->longitude = (char*) malloc(strlen(temp->longitude)+1);
					strcpy(add_temp->longitude, temp->longitude);
					add_temp->next = is_added.head;
					is_added.head = add_temp;
					
					if(strcmp(temp->city, city_name) == 0)
						filler(buf, temp->district, NULL, 0);
				}
				temp = temp->next;
			}
			free(city_name);
		}
		else if(strstr(path, codes_path) != NULL){ // Should contain files
			char* plate = (char*) malloc(3);
			strcpy(plate, path + 7);
			plate[3] = '\0';
			crow *temp = my_list.head;
			char* temp_code_name = (char*) malloc(3);

			while(temp != NULL){
				if(temp->code == NULL){
					temp = temp->next;
					continue;
				}
				
				for(i = 0; i < 2; i++)
					temp_code_name[i] = temp->code[i];
				temp_code_name[2] = '\0';
				
				crow* temp2 = my_list.head;
				while(temp2 != NULL && temp2!=temp){
					if(temp2->code == NULL){
						temp2 = temp2->next;
						continue;
					}
					if(strcmp(temp->code, temp2->code) == 0)
						break;
						
					temp2 = temp2->next;
				}
				
				if(temp==temp2 && strcmp(temp_code_name, plate) == 0){
					filler(buf, temp->code, NULL, 0);
				}
				temp = temp->next;
			}
			
			free(plate);
		}
		else{
			return -ENOENT;
		}
	}
	else if(delim_count == 3){ // /NAMES/Istanbul/Sariyer
		if(strstr(path, names_path) != NULL){ // Should contain files
			char* token_path = (char*)malloc(strlen(path)+1);
			strcpy(token_path, path);
			char* token = strtok(token_path, "/");
			
			islist is_added;
			is_added.head = NULL;
			
			char* city;
			char* district;
			
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 2)
					city = token;
				if(i == 3)
					district = token;
				token = strtok(NULL, "/");
			}
			
			crow *temp = my_list.head;
			
			while(temp != NULL){
				if(temp->city == NULL){
					temp = temp->next;
					continue;
				}
				
				crow* is_temp = is_added.head;
				while(is_temp != NULL){
					if(is_temp->city == NULL){
						is_temp = is_temp->next;
						continue;
					}
					
					if(strcmp(temp->city, is_temp->city) == 0 && strcmp(temp->district, is_temp->district) == 0 && strcmp(temp->neighborhood, is_temp->neighborhood) == 0)
						break;
						
					is_temp = is_temp->next;
				}
				if(is_temp == NULL){
					crow* add_temp = (crow*)malloc(sizeof(crow));
					add_temp->code = (char*) malloc(strlen(temp->code)+1);
					strcpy(add_temp->code, temp->code);
					add_temp->neighborhood = (char*) malloc(strlen(temp->neighborhood)+1);
					strcpy(add_temp->neighborhood, temp->neighborhood);
					add_temp->city = (char*) malloc(strlen(temp->city)+1);
					strcpy(add_temp->city, temp->city);
					add_temp->district = (char*) malloc(strlen(temp->district)+1);
					strcpy(add_temp->district, temp->district);
					add_temp->latitude = (char*) malloc(strlen(temp->latitude)+1);
					strcpy(add_temp->latitude, temp->latitude);
					add_temp->longitude = (char*) malloc(strlen(temp->longitude)+1);
					strcpy(add_temp->longitude, temp->longitude);
					add_temp->next = is_added.head;
					is_added.head = add_temp;
					
					if(strcmp(temp->city, city) == 0 && strcmp(temp->district, district) == 0){
						char* file_name = (char*) malloc(strlen(temp->neighborhood) + 5); // 5 for .txt\0
						strcpy(file_name, temp->neighborhood);
						strcat(file_name, ".txt");
						filler(buf, file_name, NULL, 0);
					}
				}
				temp = temp->next;
			}			
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
	
	if(strstr(path, names_path) != NULL){
		if(delim_count == 4){
			printf("ENTERED\n"); //DEBUG
			char* token_path = (char*)malloc(strlen(path)+1);
			strcpy(token_path, path);
			char* token = strtok(token_path, "/");
			
			char* city;
			char* district;
			char* file_name;
			
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 2)
					city = token;
				if(i == 3)
					district = token;
				if(i == 4)
					file_name = token;
				token = strtok(NULL, "/");
			}
			
			//i = 0;
			char* neighborhood = strtok(file_name, ".");
			
			crow *temp = my_list.head;
			
			
			while(temp != NULL){
				if(temp->city == NULL){
					temp = temp->next;
					continue;
				}
				
				if(strcmp(temp->city, city) == 0 
					&& strcmp(temp->district, district) == 0
					&& strcmp(temp->neighborhood, neighborhood) == 0){
						
						char* result = (char*) malloc(
								strlen(fcode) + strlen(fneighborhood) +
								strlen(fcity) + strlen(fdistrict) +
								strlen(flatitude) + strlen(flongitude) +
								strlen(temp->code) + strlen(temp->neighborhood) +
								strlen(temp->city) + strlen(temp->district) +
								strlen(temp->latitude) + strlen(temp->longitude) + 1);
						
						strcpy(result, fcode); strcat(result, temp->code);
						strcat(result, fneighborhood); strcat(result, temp->neighborhood);
						strcat(result, fcity); strcat(result, temp->city);
						strcat(result, fdistrict); strcat(result, temp->district);
						strcat(result, flatitude); strcat(result, temp->latitude);
						strcat(result, flongitude); strcat(result, temp->longitude);
						
						len = strlen(result);
						printf("len is : %d\n", len); // DEBUG
						if(offset < len){
							if(offset + size > len)
								size = len - offset;
							memcpy(buf, result + offset, size);
						}else
							size = 0;
							
						return size;
				}
				
				temp = temp->next;
			}	
		}else{
			return -ENOENT;
		}
	}
	else if(strstr(path, codes_path) != NULL){
		if(delim_count == 3){
			printf("ENTERED\n"); //DEBUG
			
			char* token_path = (char*)malloc(strlen(path)+1);
			strcpy(token_path, path);
			char* token = strtok(token_path, "/");
			
			char* code;
		
			
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 3)
					code = token;
				token = strtok(NULL, "/");
			}
			printf("pahtttt is %s\n", path);

			printf("code is %s\n", code);
			
			crow *temp = my_list.head;
			char* temp_code_name = (char*) malloc(3);
			size_t file_length = 0;

			while(temp != NULL){
				if(temp->code == NULL){
					temp = temp->next;
					continue;
				}
			
				if(strcmp(temp->code, code) == 0 ){
						
						
					file_length += strlen(fcode) + strlen(fneighborhood) +
						strlen(fcity) + strlen(fdistrict) +
						strlen(flatitude) + strlen(flongitude) +
						strlen(temp->code) + strlen(temp->neighborhood) +
						strlen(temp->city) + strlen(temp->district) +
						strlen(temp->latitude) + strlen(temp->longitude) + 3;
					
				}
				
				temp = temp->next;
			}
			crow *temp2 = my_list.head;
			printf("file length is %d\n",file_length);
			char* result = (char*) malloc(file_length + 1);
			strcpy(result, fcode); 
			printf("result is nothing %s", result);
			i = 0;
			while(temp2 != NULL){
				if(temp2->code == NULL){
					temp2 = temp2->next;
					continue;
				}
				
				
				if(strcmp(temp2->code, code) == 0){

						
						if(i != 0){
							strcat(result, fcode);
						}
						else 
							i = 1;
						strcat(result, temp2->code);
						strcat(result, fneighborhood); strcat(result, temp2->neighborhood);
						strcat(result, fcity); strcat(result, temp2->city);
						strcat(result, fdistrict); strcat(result, temp2->district);
						strcat(result, flatitude); strcat(result, temp2->latitude);
						strcat(result, flongitude); strcat(result, temp2->longitude);
						strcat(result, "\n\n");
						
				}
				
				temp2 = temp2->next;
			}
			len = strlen(result);
			printf("len is : %d\n", len); // DEBUG
			if(offset < len){
				if(offset + size > len)
					size = len - offset;
				memcpy(buf, result + offset, size);
			}else
			size = 0;
			free(result);		
			return size;
			
		}
		else{
			return -ENOENT;
		}
	}
	else{
		return -ENOENT;
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

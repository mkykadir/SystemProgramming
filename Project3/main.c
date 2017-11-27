/*
 * Muhammed Kadir Yücel - 150140119
 * Mahmut Lutfullah Özbilen - 150140123
 * 
 * Istanbul Technical University System Programming 3rd Project
 * FUSE implementation
 * 
 * Make sure you have installed libcsv and libfuse before compiling this program.
 * 
 * ./program <input_file_name>
 * 
 */

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

void csv_cleaner(){
	printf("entered cleaner\n");
	crow* clean_list = my_list.head;
	while(clean_list != NULL){
		printf("entered loop\n");
		if(clean_list->code != NULL)
			free(clean_list->code);
		if(clean_list->neighborhood != NULL)
			free(clean_list->neighborhood);
		if(clean_list->city != NULL)
			free(clean_list->city);
		if(clean_list->district != NULL)
			free(clean_list->district);
		if(clean_list->latitude != NULL)
			free(clean_list->latitude);
		if(clean_list->longitude != NULL)
			free(clean_list->longitude);
		
		printf("finished fields\n");
		crow* temp = clean_list;
		clean_list = clean_list->next;
		free(temp);
	}
	printf("exitting clean\n");
}

void csv_data_updater(){
	
	my_list.head = NULL;
	FILE* fp;
	struct csv_parser p;
	char buf[1024];
	size_t bytes_read;
	unsigned char options = CSV_STRICT | CSV_APPEND_NULL;
	
	crow* first = (crow*)malloc(sizeof(crow));
	my_list.head = first;
	first->next = NULL;
	
	if(csv_init(&p, options) != 0){
		printf("Failed to initialize csv parser\n");
		exit(EXIT_FAILURE);
	}
	
	csv_set_space_func(&p, is_space);
	csv_set_term_func(&p, is_term);
	csv_set_delim(&p, '\t');
	fp = fopen("postal-codes.csv", "rb");
	if(!fp){
		printf("Failed to open file %s\n", strerror(errno));
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
}

void csv_writer(){
	FILE* fp;
	fp = fopen("postal-codes.csv", "w");
	if(!fp)
		exit(EXIT_FAILURE);
		
	crow* temp = my_list.head;
	while(temp != NULL){
		if(temp->city == NULL){
			temp = temp->next;
			continue;
		}
		
		fprintf(fp, "%s\t%s\t%s\t%s\t%s\t%s\t\n", temp->code, temp->neighborhood, temp->city, temp->district, temp->latitude, temp->longitude);
		
		temp = temp->next;
	}
	
	fclose(fp);
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
			stbuf->st_mode = S_IFREG | 0664;
			stbuf->st_nlink = 1;
			
			char* token_path = (char*)malloc(strlen(path)+1);
			strcpy(token_path, path);
			char* token = strtok(token_path, "/");
			
			char* file_name;
			char* plate;
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 2)
					plate = token;
				if(i == 3)
					file_name = token;

				token = strtok(NULL, "/");
			}
			
			char* code = strtok(file_name, ".");
			
			size_t file_length = 0;
			crow *temp = my_list.head;
			while(temp != NULL){
				if(temp->code == NULL){
					temp = temp->next;
					continue;
				}
				
				if(strcmp(temp->code, code) == 0 ){
						
						
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
			printf("LENGTH IS %d\n", file_length);
			stbuf->st_size = file_length;
		}
		else{
			res = -ENOENT;
		}
	}
	else if(delim_count == 4){ // /NAMES/Istanbul/Sariyer/Maslak.txt
		if(strstr(path, names_path) != NULL){ // /NAMES/Istanbul/Sariyer/Maslak.txt
			stbuf->st_mode = S_IFREG | 0664;
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
			printf("LENGTH IS %d\n", file_length);
			stbuf->st_size = file_length;
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
		csv_data_updater();
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
		crow* clean_list = is_added.head;
		while(clean_list != NULL){
			if(clean_list->code)
				free(clean_list->code);
			if(clean_list->neighborhood)
				free(clean_list->neighborhood);
			if(clean_list->city)
				free(clean_list->city);
			if(clean_list->district)
				free(clean_list->district);
			if(clean_list->latitude)
				free(clean_list->latitude);
			if(clean_list->longitude)
				free(clean_list->longitude);
			
			crow* temp = clean_list;
			clean_list = clean_list->next;
			free(temp);
		}
	}else if(strcmp(path, codes_path) == 0){
		filler(buf, ".", NULL, 0);
		filler(buf, "..", NULL , 0);
		char* plate = (char*) malloc(3);
		strcpy(plate, path + 7);
		plate[3] = '\0';
		crow* temp = my_list.head;
		char* temp_code = (char*) malloc(3);
		char* temp_code2 = (char*) malloc(3);

		islist is_added;
		is_added.head = NULL;
		while(temp != NULL){
			if(temp->code == NULL){
				temp = temp->next;
				continue;
			}
			
			for(i = 0; i < 2; i++)
					temp_code[i] = temp->code[i];
			temp_code[2] = '\0';
			
			crow* is_temp = is_added.head;
			while(is_temp != NULL){
				if(is_temp->code == NULL){
					is_temp = is_temp->next;
					continue;
				}
				
				
				for(i = 0; i < 2; i++)
						temp_code2[i] = is_temp->code[i];
				temp_code2[2] = '\0';
				if(strcmp(temp_code, temp_code2) == 0)
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
				
				filler(buf, temp_code, NULL, 0);
			}
			temp = temp->next;
		}
		
	}else if(delim_count == 2){ // /NAMES/Istanbul or /CODES/34
		if(strstr(path, names_path) != NULL){ // Should containt folders
			filler(buf, ".", NULL, 0);
			filler(buf, "..", NULL , 0);
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
			crow* clean_list = is_added.head;
			while(clean_list != NULL){
				if(clean_list->code)
					free(clean_list->code);
				if(clean_list->neighborhood)
					free(clean_list->neighborhood);
				if(clean_list->city)
					free(clean_list->city);
				if(clean_list->district)
					free(clean_list->district);
				if(clean_list->latitude)
					free(clean_list->latitude);
				if(clean_list->longitude)
					free(clean_list->longitude);
				
				crow* temp = clean_list;
				clean_list = clean_list->next;
				free(temp);
			}
			free(city_name);
		}
		else if(strstr(path, codes_path) != NULL){ // Should contain files
			filler(buf, ".", NULL, 0);
			filler(buf, "..", NULL , 0);
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
						char* file_name = (char*) malloc(strlen(temp->code) + 5); // 5 for .txt\0
						strcpy(file_name, temp->code);
						strcat(file_name, ".txt");
						filler(buf, file_name, NULL, 0);
				}
				temp = temp->next;
			}
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
			
			char* plate;
			char* file_name;
			char* code;
			int i = 0;
			while(token != NULL){
				i++;
				if(i == 2)
					plate = token;
				if(i == 3)
					file_name = token;
				token = strtok(NULL, "/");
			}
			
			code = strtok(file_name, ".");
			
			crow *temp = my_list.head;
			
			
			while(temp != NULL){
				if(temp->code == NULL){
					temp = temp->next;
					continue;
				}
				
				if(strcmp(temp->code, code) == 0 ){
						
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
		}
		else{
			return -ENOENT;
		}
	}
	else{
		return -ENOENT;
	}
	
	
}

static int fuse_unlink(const char* path)
{
	int delim_count = 0;
	int i;
	for(i = 0; i < strlen(path); i++){
		if(path[i] == '/'){
			delim_count++;
			printf("DELIM_COUNT is: %d\n", delim_count); // DEBUG
		}
	}
	
	if(delim_count == 4){ // for NAMES
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
		
		crow* temp = my_list.head;
		
		if(temp != NULL){ // if element is in head!
			if(temp->city != NULL){
				if(strcmp(temp->city, city) == 0 
						&& strcmp(temp->district, district) == 0
						&& strcmp(temp->neighborhood, neighborhood) == 0){
					my_list.head = temp->next;
					free(temp->code);
					free(temp->city);
					free(temp->district);
					free(temp->neighborhood);
					free(temp->latitude);
					free(temp->longitude);
					free(temp);
					
					csv_writer();
					return 0;
				}
			}
		}
		
		crow* prev = my_list.head;
		
		while(prev->next != NULL){
			if(prev->next->city == NULL){
				prev=prev->next;
				continue;
			}
			
			if(strcmp(prev->next->city, city) != 0 
					|| strcmp(prev->next->district, district) != 0
					|| strcmp(prev->next->neighborhood, neighborhood) != 0){
						
				prev = prev->next;
			}else
				break;
		}
		
		if(prev->next == NULL)
			return -ENOENT;
			
		crow* prev_temp = prev->next;
		prev->next = prev->next->next;
		csv_writer();
		return 0;
			
	
	}
	else if(delim_count == 3){
		char* token_path = (char*)malloc(strlen(path)+1);
		strcpy(token_path, path);
		char* token = strtok(token_path, "/");
		
		char* file_name;
		char* plate;
		
		int i = 0;
		while(token != NULL){
			i++;
			if(i == 2)
				plate = token;
			if(i == 3)
				file_name = token;
			token = strtok(NULL, "/");
		}
		char* code = strtok(file_name, ".");
		
		crow* temp = my_list.head;
		
		if(temp != NULL){ // if element is in head!
			if(temp->code != NULL){
				if(strcmp(temp->code, code) == 0){
					my_list.head = temp->next;
					free(temp->code);
					free(temp->city);
					free(temp->district);
					free(temp->neighborhood);
					free(temp->latitude);
					free(temp->longitude);
					free(temp);
					
					csv_writer();
					return 0;
				}
			}
		}
		
		crow* prev = my_list.head;
		
		while(prev->next != NULL){
			if(prev->next->city == NULL){
				prev=prev->next;
				continue;
			}
			
			if(strcmp(prev->next->code, code) != 0){
						
				prev = prev->next;
			}else
				break;
		}
		
		if(prev->next == NULL)
			return -ENOENT;
			
		crow* prev_temp = prev->next;
		prev->next = prev->next->next;
		csv_writer();
		return 0;
		}
	
	return -ENOENT;
}

static int fuse_rename(const char* from, const char* to)
{
	
	int delim_count = 0;
	int i;
	for(i = 0; i < strlen(from); i++){
		if(from[i] == '/'){
			delim_count++;
			printf("DELIM_COUNT is: %d\n", delim_count); // DEBUG
		}
	}
	
	if(delim_count == 4){ // for NAMES
		char* token_path = (char*)malloc(strlen(from)+1);
		char* to_path = (char*)malloc(strlen(to)+1);
		strcpy(token_path, from);
		strcpy(to_path, to);
		printf("from %s to %s\n", from, to);
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
		printf("neigh %s\n", neighborhood);
		
		char* new_file;
		char* to_token = strtok(to_path, "/");
		i = 0;
		while(to_token != NULL){
			i++;
			if(i == 4)
				new_file = to_token;
			to_token = strtok(NULL, "/");
		}
		printf("new is %s\n", new_file);
		char* new_neighborhood = strtok(new_file, ".");
		crow* temp = my_list.head;
		while(temp != NULL){
			if(temp->city == NULL){
				temp=temp->next;
				continue;
			}
			
			if(strcmp(temp->city, city) == 0 
					&& strcmp(temp->district, district) == 0
					&& strcmp(temp->neighborhood, neighborhood) == 0){
						
				free(temp->neighborhood);
				temp->neighborhood = (char*) malloc(strlen(new_neighborhood)+1);
				strcpy(temp->neighborhood, new_neighborhood);
				break;
			}
			
			temp=temp->next;
		}
		if(temp == NULL)
			return -ENOENT;
		
		csv_writer();
		return 0;
			
	}
	else if(delim_count == 3){
		char* token_path = (char*)malloc(strlen(from)+1);
		char* to_path = (char*)malloc(strlen(to)+1);
		strcpy(token_path, from);
		strcpy(to_path, to);
		printf("from %s to %s\n", from, to);
		char* token = strtok(token_path, "/");
		
		
		char* plate;
		char* file_name;
		
		int i = 0;
		while(token != NULL){
			i++;
			if(i == 2)
				plate = token;
			if(i == 3)
				file_name = token;
			token = strtok(NULL, "/");
		}
		char* code = strtok(file_name, ".");
		printf("code %s\n", code);
		
		char* new_file;
		char* to_token = strtok(to_path, "/");
		i = 0;
		while(to_token != NULL){
			i++;
			if(i == 3)
				new_file = to_token;
			
			to_token = strtok(NULL, "/");
		}
		printf("new is %s\n", new_file);
		char* new_code = strtok(new_file, ".");
		crow* temp = my_list.head;
		while(temp != NULL){
			if(temp->code == NULL){
				temp=temp->next;
				continue;
			}
			
			if(strcmp(temp->code, code) == 0){
						
				free(temp->code);
				temp->code = (char*) malloc(strlen(code)+1);
				strcpy(temp->code, new_code);
				break;
			}
			
			temp=temp->next;
		}
		if(temp == NULL)
			return -ENOENT;
		
		csv_writer();
		return 0;
			
	}
	
	return -ENOENT;
}

static struct fuse_operations fuse_oper = {
	.getattr = fuse_getattr,
	.readdir = fuse_readdir,
	.open = fuse_open,
	.read = fuse_read,
	.rename = fuse_rename,
	.unlink = fuse_unlink,
};

int main(int argc, char* argv[])
{			
	return fuse_main(argc, argv, &fuse_oper, NULL);
}

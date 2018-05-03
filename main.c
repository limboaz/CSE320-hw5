#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>

pthread_t id[4];	//process list

//A struct of page table for each process. 
typedef struct {
	pthread_t tid;	//thread ID
	//need a list size. Addresses are represented by uint32_t integers. 
}pt;


//functions need to be created
//return 0 on success

void *thread(void *vargp);
void *cse320_malloc(int n);	
void *cse320_virt_to_phys(void *vir);
int kill_x(pthread_t tid);
void list();
void mem_x(pthread_t tid);
int allocate(pthread_t tid);
int read_xy(pthread_t tid, void *p);


int create(){
	int i;
	int added = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	for ( i = 0; i < 4; i++){
		if ( id[i] < 1 ){
			id[i] = tid;
			added = 1;
			break;
		}
	}
	return added;
}

void *thread(void *vargp){
	//when creating a thread, create a corresponding page table. 
	
}


void main(){
	fputs("Please select from the following option\ncreate\nkill X\nlist\nmem X\nallocate X\nread X Y\nwrite X Y Z\nexit\n", stdout);
	char *args[5];
	char cmd[127];
	while(1){
		fgets(cmd, 127, stdin);
		int i;
		for ( i = 0; i < 127; i++){
			if ( cmd[i] == '\n')
				cmd[i] = '\0';
			break;
		}
		if (strcmp(cmd, "create") == 0){
		
		}else if (strcmp(cmd, "exit") == 0){
			exit(0);
		}else if (strcmp(cmd, "list") == 0){
			//list all current threads
		}else{
			char *p = strtok(cmd, " ");
			p = strtok(NULL, " ");
			int j;
			for (j = 0; j < 4; j++){
				*(args + j) = p;
				p = strtok(NULL, " ");
			}
			*(args + 4) = NULL;
			if (strncmp(cmd, "kill", 4) == 0){
				//kill thread X stored in args[1]
			}else if(strncmp(cmd, "mem", 3) == 0){
				//list virtual addresses used by X
			}else if(strncmp(cmd, "allocate", 8) == 0){
				//add the address to list
			}else if(strncmp(cmd, "read", 4) == 0){
				//
			}else if(strncmp(cmd, "write", 5) == 0){
				//write 
			}
		
		}
	}
}

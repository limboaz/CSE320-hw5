#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

pthread_t id[4];	//process list

//level 1 page table.
typedef struct {
	pthread_t tid;	//thread ID
	int pte;
}pt1;

//so we build a level 1 page table. 
pt1 lev1[1024];

//level 2 page table. 
typedef struct {
	int pte1;	//level 1 page table entry
	int pte[1024];	//values stored are the index of bytes in memory. 
}pt2;

//and we build 1024 level 2 page table. 
pt2 lev2[1024];

//functions need to be created
//return 0 on success

void *thread(void *vargp);		//done
void *cse320_malloc(int n);	
void *cse320_virt_to_phys(void *vir);
int kill_x(pthread_t tid);	//done
void list();	//done
void mem_x(pthread_t tid);	//done
int allocate(pthread_t tid);
int read_xy(pthread_t tid, void *p);
int write_xyz(pthread_t tid, int addr, int var);


int create(){
	int i;
	int added = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	for ( i = 0; i < 4; i++){
		if ( id[i] < 1 ){
			id[i] = tid;
			lev1[i].pte = i;	//no more than 4 threads in total so we can do this.
			lev1[i].tid = tid;	//added to first level page table. 
			lev2[i].pte = i;	//assign a lev2 pt to this process. 
			added = 1;
			break;
		}
	}
	return added;
}

void *thread(void *vargp){
	//when creating a thread, create a corresponding page table. 
	//this is done in create function. 
}

int kill_X(pthread_t tid){
	int killed = 0;
	pthread_cancel(tid);
	//clear corresponding page table. 
	int i;
	for ( i = 0; i < 4; i++){
		if( id[i] == tid && lev1[i].tid == tid ){
			id[i] = 0;
			lev1[i].tid = 0;
			lev2[lev1[i].pte]
			lev1[i].pte = -1;	//to indicate unused entry. 
			killed = 1;
			break;
		}
	}
	return killed;
}

//list all "processes" for current app. 
void list(){
	int i;
	int printed = 0;
	for ( i = 0; i < 4; i++){
		if(id[i] != 0 ){
			if ( i == 0)
				fputs("Process Number	Process ID\n", stdout);
			char arr[4];
			sprintf(arr, "%d", i);
			char tid_s[128];
			sprintf(tid_s, "%lu", id[i]);
			fputs(arr, stdout);
			fputs("\t", stdout);
			fputs(tid_s, stdout);
			fputs("\n", stdout);	//just formatting
			printed++;
		}
	}
	if (printed == 0)
		fputs("No process is currently running. \n", stdout);
}

int mem_X(pthread_t tid){
	int found = 0;
	int i;
	for ( i = 0; i < 4; i++){
		if( lev1[i].tid == tid){
			found = 1;	//if tid is found in current list, the value of i should be its index in lev1 pt. 
			break;
		}
	}
	if ( found != 1){
		fputs("Process does not exist.\n", stdout);
		return -1;
	}else{
		uint32_t addr = 0;
		//put first level pte
		addr = lev1[i].pte << 22;
		int j;	//iterate the second level page table. 
		for ( j = 0; j < 1024; j++){
			
		}

	}
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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>

//level 2 page table. 
typedef struct {
	int val;	//used or not
	int phys;	//store where it actually points to #block in physical memory. 
	int pte;	//values stored are the index of bytes in memory. 
}pt2;

typedef struct {
	pthread_t tid;	//thread
	//use the index of array to represent pt1 entry so no need to add here
	pt2 lev2[1024];
}pt1;

pt1 lev1[4];

//functions need to be created
//return 0 on success

void *thread(void *vargp);		//done
int cse320_malloc(int pte);		//done
int cse320_virt_to_phys(int vir);
int kill_x(pthread_t tid);	//done
void list();	//done
void mem_x(pthread_t tid);	//done
int allocate_x(pthread_t tid);	//done
int read_xy(pthread_t tid, int addr);	//done
int write_xyz(pthread_t tid, int addr, int var);

int cse320_malloc(int pte){
	int addr = 0;
	addr = pte << 22;
		int j;
		for ( j = 0; j < 1024; j ++){
			if (lev1[pte].lev2[j].val == 0){
				addr = addr | ( j << 12);
				lev1[pte].lev2[j].val = 1;
				return addr;	//return the allocated address if generated
			}
		}
		return 0;
}

int cse320_virt_to_phys(int vir){
	int p1;
	int p2;
	p1 = vir >> 22;
	p2 = (vir << 10) >> 22;
}

int create(){
	int i;
	int added = 0;
	pthread_t tid;
	pthread_create(&tid, NULL, thread, NULL);
	for ( i = 0; i < 4; i++){
		if ( lev1[i].tid < 1 ){
			lev1[i].tid = tid;	//added to first level page table. 
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
		if( lev1[i].tid == tid ){
			lev1[i].tid = 0;
			int j;
			for ( j = 0; j < 1024; j++)
				lev1[i].lev2[j].val = 0;	//clear pt2 as well 
			killed++;
			break;
		}
	}
	if ( killed != 1 ){
		fputs("error when killing process X.\n", stdout);
	}
	return killed;
}

//list all "processes" for current app. 
void list(){
	int i;
	int printed = 0;
	for ( i = 0; i < 4; i++){
		if(lev1[i].tid != 0 ){
			if ( i == 0)
				fputs("Process Number	Process ID\n", stdout);
			char arr[4];
			sprintf(arr, "%d", i);
			char tid_s[128];
			sprintf(tid_s, "%lu", lev1[i].tid);
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
	int i;	//index of the corresponding pt1
	for ( i = 0; i < 4; i++){
		if( lev1[i].tid == tid){
			found = 1;	//if tid is found in current list, the value of i should be its index in lev1 pt. 
			break;
		}
	}
	if ( found != 1){
		fputs("Process does not exist.\n", stdout);
		return found;
	}else{
		fputs("Addesses used by X:\n", stdout);
		uint32_t addr = 0;
		//put first level pte
		addr = i << 22;	//i represents pt1 entry
		int j;	//iterate the second level page table. 
		for ( j = 0; j < 1024; j++){
			if ( lev1[i].lev2[j].val != 0 )	//j here can also represent entry num, as for pt1. 
				addr = ( addr | (j << 12));	//one address is generated. 
				printf("%x\n", addr);
		}
	}
	return found;
}

int allocate_X(pthread_t tid){
	int i;
	int addr;
	int found = 0;
	for ( i = 0; i < 4; i++)
		if ( lev1[i].tid == tid){
			found = 1;
			break;
		}
	if ( found != 1){
		fputs("Process not found. \n", stdout);
	}else{
		return cse320_malloc(i);
	}
}

int read_xy(pthread_t tid, int addr){
	//read 
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

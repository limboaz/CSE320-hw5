#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

//level 2 page table. 
typedef struct {
	int val;	//used or not
	int phys;	//store where it actually points to #block in physical memory. 	
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
int cse320_malloc(int pte, int phy);		//done
int cse320_virt_to_phys(int vir);	//done
int kill_x(pthread_t tid);	//done
void list();	//done
void mem_x(pthread_t tid);	//done
int allocate_x(pthread_t tid);	//done
int read_xy(pthread_t tid, int addr);	//done
int write_xyz(pthread_t tid, int addr, int var);	//done

//this function is from stackoverflow
void print_binary(int num){
	int cnt;
	for ( cnt = 0; cnt < 32; cnt ++){
		printf("%d", (num >> (31 - cnt)) & 1 );
	}
	printf("\n");
}

int cse320_malloc(int pte, int phy){
	int addr = 0;
	addr = pte << 22;
	int j;
	for ( j = 0; j < 1024; j ++){
		if (lev1[pte].lev2[j].val == 0){
			addr = addr | ( j << 12);
			lev1[pte].lev2[j].val = 1;	
			lev1[pte].lev2[j].phys = phy;
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
	int addr;
	addr = p1 * 256 + p2;
	return addr;
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
			int fifo_mem, fifo_main;
			fifo_mem = open("fifo_mem", O_RDWR);
			char mes[30];
			sprintf(mes, "clean %d", i);
			write(fifo_mem, mes, 30);
			close(fifo_mem);
			fifo_main = open("fifo_main", O_RDWR);
			read(fifo_main, NULL, 0);
			close(fifo_main);
			lev1[i].tid = 0;
			int j;
			for ( j = 0; j < 1024; j++){
				lev1[i].lev2[j].val = 0;	//clear pt2 as well 
				lev1[i].lev2[j].phys = 0;
			}
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
				fputs("Process Number		Process ID\n", stdout);
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
			if ( lev1[i].lev2[j].val == 1 ){	//j here can also represent entry num, as for pt1. 
				print_binary(( addr | (j << 12)));	//one address is generated. 
			}
		}
	}
	return found;
}

int allocate_X(pthread_t tid){
	int i;
	int addr;
	int found = 0;
	for ( i = 0; i < 4; i++ )
		if ( lev1[i].tid == tid){
			found = 1;
			break;
		}
	if ( found != 1){
		fputs("Process not found. \n", stdout);
	}else{
		//addr = cse320_malloc(i);

		int fifo_mem, fifo_main;
		fifo_mem = open("fifo_mem", O_RDWR);
		char mes[20];
		sprintf(mes, "request %d", i);
		write(fifo_mem, mes, 20);
		close(fifo_mem);
		fifo_main = open("fifo_main", O_RDWR);
		char phys[20];
		read(fifo_main, phys, 20);
		close(fifo_main);
		int phy_addr = atoi(phys);
		addr = cse320_malloc(i, phy_addr);
		return addr;
	}
}

int read_xy(pthread_t tid, int addr){
	int i;
	int found = 0;
	for ( i = 0; i < 4; i++){
		if ( lev1[i].tid == tid){
			found = 1;
			break;
		}
	}
	if ( found != 1){
		printf("Process not found. \n");
		return -1;
	}
	int phys;
	char *val = malloc(128);
	int value = 0;
	phys = cse320_virt_to_phys(addr);
	char mes[128] = "read ";
	char num[30];
	sprintf(num, "%d", phys);
	strcat(mes, num);
	int fifo_main, fifo_mem;
	fifo_mem = open("fifo_mem", O_RDWR);
	write(fifo_mem, mes, 128);
	close(fifo_mem);
	fifo_main = open("fifo_main", O_RDWR);
	read(fifo_main, val, 128);
	value = atoi(val); 
	printf("%d\n", value);
	free(val);
	return value;

}

int write_xyz(pthread_t tid, int addr, int var){
	int phys = cse320_virt_to_phys(addr);
	char mes[128] = "write ";
	char ad[30];
	sprintf(ad, "%d %d", phys, var);
	strcat(mes, ad);
	int fifo_main, fifo_mem;
	fifo_mem = open("fifo_mem", O_RDWR); 
	write(fifo_mem, mes, 128);
	close(fifo_mem);
	fifo_main = open("fifo_main", O_RDWR);
	read(fifo_main, NULL, 0);
	close(fifo_main);
	return 1;
}


void main(){
	int file1, file2;
	file1 = mkfifo("fifo_main", 0666);
	if ( file1 < 0){
		fputs("Unable to create fifo_main.\n", stdout);
		exit(-1);
	}
	file2 = mkfifo("fifo_mem", 0666);
	if ( file2 < 0 ){
		fputs("Unable to create fifo_mem.\n", stdout);
		exit(-1);
	}
	//there was once some code, in memory of those effort
	fputs("Please select from the following option\ncreate\nkill X\nlist\nmem X\nallocate X\nread X Y\nwrite X Y Z\nexit\n", stdout);
	char *args[5];
	char cmd[127];
	while(1){
		fgets(cmd, 127, stdin);
		int i;
		for ( i = 0; i < 127; i++){
			if ( cmd[i] == '\n'){
				cmd[i] = '\0';
				break;
			}
		}
		if (strcmp(cmd, "create") == 0){
			create();	
		}else if (strcmp(cmd, "exit") == 0){
			exit(0);
		}else if (strcmp(cmd, "list") == 0){
			//list all current threads
			list();
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
				pthread_t tid = strtoul(args[0], NULL, 10);
				kill_X(tid);
			}else if(strncmp(cmd, "mem", 3) == 0){
				//list virtual addresses used by X
				pthread_t tid = strtoul(args[0], NULL, 10);
				mem_X(tid);
			}else if(strncmp(cmd, "allocate", 8) == 0){
				//add the address to list
				pthread_t tid = strtoul(args[0], NULL, 10);
				print_binary(allocate_X(tid));
			}else if(strncmp(cmd, "read", 4) == 0){
				pthread_t tid = strtoul(args[0], NULL, 10);
				int addr = atoi(args[1]);
				read_xy(tid, addr);
			}else if(strncmp(cmd, "write", 5) == 0){
				//write
				pthread_t tid = strtoul(args[0], NULL, 10);
				int addr = atoi(args[1]);
				int val = atoi(args[2]);
				write_xyz(tid, addr, val);
			}
		
		}
	}
}

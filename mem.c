#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>

void main(){
	//allocate 1024 bytes, then move using counters. 
	void *mem = malloc(1024);
	char *buf = malloc(128);
	//go with separating mem into 4*256. 
	int cnt[4] = {0, 256, 512, 768};
	int fifo_main, fifo_mem; 
	
	while(1){
		printf("Memory is running. \n");
		fifo_mem = open("fifo_mem", O_RDWR);
		if ( fifo_mem < 0 ){
			fputs("Error opening fifo_mem in mem\n", stdout);
			exit(-1);
		}
		fifo_main = open("fifo_main", O_RDWR);
		if ( fifo_main < 0 ){
			fputs("Error opening fifo_main in mem\n", stdout);
			exit(-1);
		}
		read(fifo_mem, buf, 128);	//read command from main
		close(fifo_mem);
		char *p = strtok(buf, " ");
		sleep(5);
		if (strncmp(buf, "read", 4) == 0){
			//read value from mem
			//passing in an actual phys addr
			p = strtok(NULL, " ");
			int addr;
			//memory is sectioned into 4 blocks but the physical address usage should be handled by main so no need to worry about it here. 
			addr = atoi(p);
			int val;
			if ( addr % 4 == 0){
				val = *((int *)(mem + addr));
				char mes[30];
				sprintf(mes, "%d", val);
				write(fifo_main, mes, 30);	//return the value to main
			}
			else {
				char mes[] = "Invalid address.\n";
				printf("%s", mes);
			}


		}else if ( strncmp(buf, "write", 5) == 0){
			//write value Y to addr X
			p = strtok(NULL, " ");
			int addr = atoi(p);
			p = strtok(NULL, " ");
			int val = atoi(p);
			//some problem here. Should also know which section to deal with => need to translate 
			if ( addr < 1024 && addr % 4 == 0 ){	//memory should not be out of bound & should be multiple of 4
				*((int *)(mem + addr)) = val;	//writing the specified value into memory. 
				printf("addr: %d val:%d\n", addr, *((int*)(mem + addr)));
			}else{
				char mes[] = "Address out of range\n";
				printf("%s", mes);
			}	
		}else if( strncmp(buf, "request", 7) == 0) {
			//write the current available phys addr to fifo_main
				p = strtok(NULL, " ");
				int pt1 = atoi(p);
				char mes[20];
				sprintf(mes, "%d", cnt[pt1]);
				cnt[pt1] += 4;
				write(fifo_main, mes, 20);
				
		}else if ( strncmp(buf, "clean", 5) == 0){
				p = strtok(NULL, " ");
				int pt1 = atoi(p);
				cnt[pt1] = pt1 * 256;
				
		}
		close(fifo_main);
	//if
	}
	free(buf);
	free(mem);		
	
}//main

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void main(){
	//allocate 1024 bytes, then move using counters. 
	void *mem = malloc(1024);
	//go with separating mem into 4*256. 
	int cnt = 0;
	int fifo_main, fifo_mem;
	fifo_main = open("fifo_main", O_RDWR);
	if ( fifo_main < 0 ){
		fputs("Error opening fifo_main in mem\n", stdout);
		exit(-1);
	}
	fifo_mem = open("fifo_mem", O_RDWR);
	if ( fifo_mem < 0 ){
		fputs("Error opening fifo_mem in mem\n", stdout);
	char *buf = malloc(128);
	read(fifo_mem, buf, 128);	//read command from main
	char *p = strtok(buf, " ");
	if (strncmp(buf, "read", 4) == 0){
		//read value from mem
		//passing in an actual phys addr
		p = strtok(NULL, " ");
		int addr;
		//memory is sectioned into 4 blocks but the physical address usage should be handled by main so no need to worry about it here. 
		addr = atoi(p);
		int val;
		if ( addr % 4 == 0){
			val = *(mem + addr);
			write(fifo_main, &val, sizeof(int));	//return the value to main
		}
		else {
			char mes[] = "Invalid address.\n";
			write(fifo_main, mes, 128);
		}


	}else if ( strncmp(buf, "write", 5) == 0){
		//write value Y to addr X
		p = strtok(NULL, " ");
		int addr = atoi(p);
		int val = atoi(p);
		//some problem here. Should also know which section to deal with => need to translate 
		if ( addr < 256 && addr % 4 == 0 ){	//memory should not be out of bound & should be multiple of 4
			*(mem + addr) = val;	//writing the specified value into memory. 
		}else{
			char mes[] = "Address out of range\n";
			write(fifo_main, mes, 128);
		}
	}
	}
	free(buf);
	free(mem);

}

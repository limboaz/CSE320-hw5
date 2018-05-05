#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

void main(){
	int file1, file2;
	file1 = mkfifo("fifo_main", 0666);
	if (file1 < 0){
		fputs("Unable to create a fifo.\n", stdout);
		exit(-1);
	}

	file2 = mkfifo("fifo_mem", 0666);
	if ( file2 < 0 ){
		fputs("Unable to create fifo.\n", stdout);
		exit(-1);
	}
	fputs("Pipes create successfully.\n", stdout);
}

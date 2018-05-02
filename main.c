#include <stdio.h>
#include <sthlib.h>
#include <string.h>
#include <unistd.h>


void main(){
	fputs("Please select from the following option\n
			create\n
			kill X\n
			list\n
			mem X\n
			allocate X\n
			read X Y\n
			write X Y Z\n
			exit\n", stdout);
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

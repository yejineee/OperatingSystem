#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "error.h"
#include "collatz.h"

bool isPositiveInt(char* num){
	int len = strlen(num);
	for(int i = 0; i < len; i++){
		if(isdigit(num[i]) == false){
			return false;
		}
	}
	return atoi(num) > 0; 
}

bool inputErrorHandler(int argc, char* argv[]){
	if(argc != 2){
		printf("%s", inputErrorMsg);
		return true;
	}else if(!isPositiveInt(argv[1])){
		printf("%s", inputRangeErrorMsg);
		return true;
	}
	return false;
}

int main (int argc, char* argv[])
{
	if(inputErrorHandler(argc, argv)){
		return 1;
	};
	int startNum = atoi(argv[1]);
	pid_t childPid ;
	
	childPid = fork() ;

	if (childPid == 0) { // child process
		printCollatzSequence(startNum);
	} 
	else if (childPid > 0) { // parent process
		wait(0x0);
	}
	else /* child_pid < 0 */ {
		fprintf(stderr, "Fork failed.\n") ;
		return 1;
	}

	return 0;
}

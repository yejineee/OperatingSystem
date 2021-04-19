#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include "error.h"
#include "collatz.h"

bool isPositiveInt(int num){
	return isdigit(num) == 0 && num > 0; 
}

bool isErrorInInput(int argc, char* argv[]){
	if(argc != 2){
		printf("%s", inputErrorMsg);
		return true;
	}else if(!isPositiveInt(atoi(argv[1]))){
		printf("%s", inputRangeErrorMsg);
		return true;
	}
	return false;
}

int main (int argc, char* argv[])
{
	if(isErrorInInput(argc, argv)){
		return 1;
	};
	int startNum = atoi(argv[1]);
	pid_t childPid ;
	
	/*
		 shared-memory object 생성하기
		 1. shm_open
		 2. ftruncate
		 3. mmap
	*/

	childPid = fork() ;

	if (childPid == 0) { // child process
		printCollatzSequence(startNum);
	} 
	else if (childPid > 0) { // parent process
		wait(0x0);
		/*
		 1. print out contents of shared memory
		 2. remove shared memory object
		*/
	}
	else /* child_pid < 0 */ {
		fprintf(stderr, "Fork failed.\n") ;
		return 1;
	}

	return 0;
}

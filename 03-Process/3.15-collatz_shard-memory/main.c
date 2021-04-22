#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
//shared memory
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include "error.h"
#include "collatz.h"

const int ERROR = -1;
const char* NAME = "/collatz";
const int SIZE = 4096;

bool isPositiveInt(char* num){
	int len = strlen(num);
	for(int i = 0; i < len; i++){
		if(isdigit(num[i]) == false){
			return false;
		}
	}
	return atoi(num) > 0; 
}

bool isErrorInInput(int argc, char* argv[]){
	if(argc != 2){
		fprintf(stderr, "%s", inputErrorMsg);
		return true;
	}else if(!isPositiveInt(argv[1])){
		fprintf(stderr, "%s", inputRangeErrorMsg);
		return true;
	}
	return false;
}

void* createSharedMemory(const char* name, int size){
	mode_t rwMode = 0666;
	int shm_fd = shm_open(name, O_CREAT | O_RDWR, rwMode);
	if(shm_fd == ERROR){
		fprintf(stderr, "%s - errno: %s \n", shmOpenErrorMsg, strerror(errno));
		return NULL;
	}
	int code = ftruncate(shm_fd, size);
	if(code == ERROR){
		fprintf(stderr, "%s - errno: %d, %s \n", ftruncateErrorMsg, errno, strerror(errno));
		return NULL;
	}
	void* ptr = mmap(0, size, PROT_READ, MAP_SHARED, shm_fd, 0);
	if(ptr == MAP_FAILED){
		fprintf(stderr, "%s - errno: %s \n", mmapErrorMsg, strerror(errno));
		return NULL;
	}
	return ptr;
}

void* openSharedMemory(const char* name, int size){
	mode_t rwMode = 0666;
	int shm_fd = shm_open(name, O_RDWR, rwMode);
	if(shm_fd == ERROR){
		fprintf(stderr, "%s - errno: %s \n", shmOpenErrorMsg, strerror(errno));
		return NULL;
	}
	void* ptr = mmap(0, size, PROT_WRITE, MAP_SHARED, shm_fd, 0);
	if(ptr == MAP_FAILED){
		fprintf(stderr, "%s - errno: %s \n", mmapErrorMsg, strerror(errno));
		return NULL;
	}
	return ptr;
}

bool clearSharedMemory(const char* name, int size){
	if(shm_unlink(NAME) == ERROR){
		fprintf(stderr, "%s - errno: %s \n", shmUnlinkErrorMsg, strerror(errno));
		return false;
	};
	if(munmap(0, SIZE) == ERROR){
		fprintf(stderr, "%s - errno: %s \n", munmapErrorMsg, strerror(errno));
		return false;
	}
	return true;
}


int main (int argc, char* argv[])
{
	if(isErrorInInput(argc, argv)){
		return 1;
	};
	int startNum = atoi(argv[1]);
	pid_t childPid ;

	void* mmapPtr = createSharedMemory(NAME, SIZE);
	if(mmapPtr == NULL){
		return 1;
	}

	childPid = fork() ;

	if (childPid == 0) { // child process
		void* mmapPtr = openSharedMemory(NAME, SIZE);
		char buf[SIZE];
		bufCollatzSequenceStr(startNum, buf, SIZE);
		snprintf(mmapPtr, SIZE, "%s", buf);
	} 
	else if (childPid > 0) { // parent process
		wait(0x0);
		printf("%s", (char*)mmapPtr);
		if(!clearSharedMemory(NAME, SIZE)){
			return 1;
		}
	}
	else /* child_pid < 0 */ {
		fprintf(stderr, "Fork failed.\n") ;
		return 1;
	}

	return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <stdbool.h>


typedef long long ll;

void childProcess(int num){
}

bool isPositiveInt(int num){
	return isdigit(num) != false  && num > 0; 
}

bool isInputError(int argc, char* argv[]){
	
}

int main (int argc, char* argv[])
{
	isInputError(argc, argv);
	pid_t child_pid ;
	
	child_pid = fork() ;
	if (child_pid == 0) { // child process
	} 
	else if (child_pid > 0) { // parent process
	}
	else /* child_pid < 0 */ {
		fprintf(stderr, "Fork failed.\n") ;
		return 1;
	}

	return 0;
}

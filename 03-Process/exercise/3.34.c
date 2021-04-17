#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>

#define SIZE 5

int nums[SIZE] = {0, 1, 2, 3,4};

int main(){
		int i ;
		pid_t pid;

		pid = fork();

		if(pid == 0){
				for(i = 0; i < SIZE ; i++){			
						nums[i] *= -i;
						printf("Child : %d\n", nums[i]); // line X
				}
		}else if(pid > 0){
				wait(NULL);
				for(i = 0; i < SIZE ; i++){
						printf("Parent : %d\n", nums[i]); // line Y
				}
		}
		return 0;
}

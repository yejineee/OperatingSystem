#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>

int main(){
	const int SIZE = 4096;
	const char name[] = "OS";
	const char msg0[] = "hello";
	const char msg1[] = "world!\n";
	
	int shm_fd;
	void *ptr;
	
	shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

	ftruncate(shm_fd, SIZE);

	ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);	
	if(ptr == MAP_FAILED){
		fprintf(stderr, "mmap failed\n");
		return 1;
	}

	sprintf(ptr, "%s", msg0);
	ptr += strlen(msg0);	
	sprintf(ptr, "%s", msg1);
	
	return 0;
}

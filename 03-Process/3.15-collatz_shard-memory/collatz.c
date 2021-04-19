#include <stdio.h>
#include <string.h>

long long collatz(long long num){
  return num % 2 == 0 ? num / 2 :  num * 3 + 1;
}

void printCollatzSequence(long long num){
	printf("Collatz Sequence : %lld\t", num);
	
	while(num != 1){
		num = collatz(num);
		printf("%lld\t", num);
	}
	puts("");
}	

void bufCollatzSequenceStr(long long num, char* buf, int maxSize){
	int n = snprintf(buf, maxSize, "Collatz Sequence : %lld\t", num);
	while(num != 1){
		num = collatz(num);
		n += snprintf(buf+n, maxSize-strlen(buf), "%lld\t", num);
	}
	snprintf(buf+n, maxSize-strlen(buf), "\n");
}
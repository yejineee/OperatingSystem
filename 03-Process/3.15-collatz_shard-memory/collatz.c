#include <stdio.h>
long long collatz(long long num){
  return num % 2 == 0 ? num / 2 :  num * 3 + 1;
}

void printCollatzSequence(long long num){
	printf("Collatz Sequence : %lld\t", num);
	
	while(num != 1){
		num = collatz(num);
		// printf("%lld\t", num);
		/*
		shared memory에 num 쓰기
		*/
	}
	puts("");
}	

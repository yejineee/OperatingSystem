
# C - 타입 한정자, 지정자

## 타입 한정자 - const, volatile, restrict

### const 

- 변수에 `const`를 붙이면, 해당 변수는 변하지 않음을 나타낸다. 
- 배열에 `const`가 있다면, 배열의 요소는 변하지 않음을 나타낸다.
- 함수의 배열 파라미터로 `const`가 쓰인다면, 해당 함수는 배열을 변경시키지 않을 것을 나타낸다.

	```c
	const double e = 124.1346;
	const char msg[] = "warning: \n";
	int firstPositive(const int* arr, int n)
	
	e = 1.0; //error
	msg[0] = 'c'; // error
	```
- const를 **포인터 변수**에 사용할 때는 위치에 따라 상수의 의미가 달라진다. 
	1. 포인터 변수를 사용하여 대상을 변경하는 것이 불가
		```c
		int num = 10;
		const int *ptr = &num;
		*ptr = 20; // error
		num = 20;  // 가능
		```
		포인터 변수 ptr로 num에 접근하여 값을 변경하는 것이 불가능하다. 그러나 변수 num 자체가 상수가 되는 것은 아니다. 포인터 변수 없이 바로 num에 접근하여 변경하는 것은 가능하다.
		
		다음과 같은 코드는 가능하다.
		포인터 변수가 상수가 아니므로, 가리키는 값을 변경시킬 수 있는 것이다.
		포인터 변수가 상수가 되려면...? 2번을 확인해보자.
		```c
		const char* str = "str";
    	str = "new str";
		```
		
	3. 포인터 변수가 상수가 되는 것
		`const`를 포인터 변수 이름 앞에 붙이면, 포인터 변수 자체가 상수가 된다.
		이는 포인터 변수가 가리키는 주소값이 변경되면 안된다는 것이다. 
		```c
		char* const str = "str";
		str = "new str"; // error: assignment of read-only variable ‘str’
		```
		위의 코드처럼 포인터 변수를 상수로 만들면, 포인터 변수인 str을 새로운 주소값을 가리키게 하면 에러가 발생한다.

## 지정자 - auto, extern, static, register

### static 

- static 전역 변수
	- static이 전역 변수에 쓰이면, 해당 변수의 scope는 해당 소스 파일로 제한된다.
	- 함수의 선언에도 static이 붙을 수 있다. 함수는 기본적으로 전역변수이므로, 외부의 파일에서 해당 함수를 사용할 수 있다. 그러나, static 함수는 해당 파일 내에서만 사용할 수 있으며, 외부에는 보이지 않게 된다. 
	- 소스 파일에서 외부에 쓰이는 external 변수나 함수가 있을 때, static 전역변수는 숨길 수 있다.
	- 같은 프로그램의 다른 파일에 있는 전역 변수와 이름이 같아서 충돌될 일이 없다. 
	
	![](https://i.imgur.com/nnC55xq.png)


- static 로컬 변수
	- 함수 내에 있는 변수에 static이 붙으면, 해당 변수의 scope는 여전히 해당 함수 내부이다.
	- 그런나 static 로컬 변수의 값은 함수의 호출과 리턴과 관계없이 영구적이다.
	- Internal static variables provide private, permanent storage within a single function.
	
	```c
	void increaseNumber()
	{
		static int num1 = 0;     // 정적 변수 선언 및 값 초기화
		printf("%d\n", num1);    // 정적 변수 num1의 값을 출력
		num1++;    // 정적 변수 num1의 값을 1 증가시킴
	}

	int main()
	{
		increaseNumber();    // 0
		increaseNumber();    // 1
		increaseNumber();    // 2
		increaseNumber();    // 3: 정적 변수가 사라지지 않고 유지되므로 값이 계속 증가함

		return 0;
	}
	```
	
## 출처
- The C Programming Language 2nd - Brian W.Kernighan, Dennis M.Ritchie
- [79.2 정적 변수 선언하기 - c언어 코딩도장](https://dojang.io/mod/page/view.php?id=690)
- 윤성우의 열혈 C 프로그래밍


# [03. Process] Programming Problems

## \[3.15] 콜라츠 추측(Collatz conjecture) - Shared Memory

### 📝 문제 

![](https://i.imgur.com/iVyh7Z9.png)

프로그래밍 문제 3.14를 shared memory를 사용하는 프로그램으로 수정하는 문제이다.

다음을 만족하는 C 프로그램을 작성하라.

- 커맨드 라인으로 시작 숫자를 입력 받는다.
- 커맨드 라인으로 받은 숫자가 양의 정수인지 확인하는 에러 체크가 필요하다.
- **자식 프로세스는 콜라츠 추측의 시퀀스를 shared-memory에 작성**한다.
- 부모 프로세스는 자식 프로세스의 실행이 끝났을 때, 그 결과를 출력한다.
- **부모 프로세스는 POSIX shared memory를 만들어야 한다.**


### ⚙️ 실행 방법

`make`로 컴파일 후, 커맨드 라인으로 양의 정수 하나를 입력한다.

```
$ make
$ ./main {positive_integer}
```



### ✔️ 결과 



![](https://i.imgur.com/9AtJuGD.png)




### 👩🏻‍💻 What I Learned 

#### 1. errno와 strerror로 에러 메시지 출력하기
shared memory를 구성할 때, `shm_open()` 을 사용하게 된다.
이 때, 에러가 발생하면 -1을 반환하고, errno에 어떤 에러인지가 set된다. 
errno은 숫자지만, errno에 대한 메시지를 보고 싶을 땐 **`char* strerror(int errnum)`** 을 사용하면 된다. 
strerror함수를 통해 어떤 에러인지를 더 잘 확인할 수 있게 된다.

- 에러핸들링 예
```c
if(shm_fd == ERROR){
	fprintf(stderr, "%s - errno: %d - %s \n", shmOpenErrorMsg, errno, strerror(errno));
}
```
- 결과
```
error at shm_open()  - errno: 13 - Permission denied
```

#### 2. string을 담은 배열의 주소값을 반환한다?

리팩토링을 하면서 자식 프로세스가 char형 문자를 버퍼에 담는 부분을 함수로 분리하고자 하였다.
기존에는 버퍼로 사용할 배열을 함수 내에서 만든 후, 그 주소값을 반환하였다.
```c
char* getCollatzSequenceStr(long long num, int maxSize){
	char buf[maxSize];
	int n = snprintf(buf, maxSize, "Collatz Sequence : %lld\t", num);
	while(num != 1){
		num = collatz(num);
		n += snprintf(buf+n, maxSize-strlen(buf), "%lld\t", num);
	}
	snprintf(buf+n, maxSize-strlen(buf), "\n");
	return buf;
}
```
이 코드의 문제는 함수 내부의 메모리 주소를 리턴하는데 있다.
이 함수가 리턴된 후에는 이 함수 내부의 데이터가 담긴 stack frame이 pop되면서, 데이터는 메모리 상에 없어지게 된다.

이 문제를 해결하기 위해, 해당 함수를 호출하는 쪽에서 buf 배열을 만들고, 그 주소값을 넘겨주었다.
함수가 리턴되더라도, 함수를 호출한 쪽의 데이터는 메모리에 남아있어서, 성공적으로 버퍼링한 데이터를 확인할 수 있었다.

```c
void bufCollatzSequenceStr(long long num, char* buf, int maxSize){
	int n = snprintf(buf, maxSize, "Collatz Sequence : %lld\t", num);
	while(num != 1){
		num = collatz(num);
		n += snprintf(buf+n, maxSize-strlen(buf), "%lld\t", num);
	}
	snprintf(buf+n, maxSize-strlen(buf), "\n");
}
// caller
char buf[SIZE];
bufCollatzSequenceStr(startNum, buf, SIZE);
snprintf(mmapPtr, SIZE, "%s", buf);
```


#### 3. string의 lifetime

![](https://i.imgur.com/Uw0MNNi.png)

```c=
char* str = "hello\n";
```
char 포인터 변수가 가리키는 "hello\n"는 code 영역(text 영역이라고도 한다)의 `.rodata` 영역에 저장된다. 이 영역은 프로그램이 실행하기 전에 지정되는 영역이다.

코드 영역은 stack 영역과는 달리, 프로그램이 종료될 때까지 살아있는 영역이다.

```c
int function(){
	char* ptr = "in rodata";
}
```
이 함수가 종료되어도, "in rodata"는 메모리 영역에 남아있다는 뜻이다. 
"in rodata"는 read-only data영역에 존재하므로, `ptr`로 이 문자열을 변경시킬 수 없다.

문자열을 변경시키려면, char을 문자열 배열에 담아야한다.
```c
char str [] = "hello";
```
이 때 "hello"는 초기에 `.rodata`에 담기는데, 프로그램이 시작하고 나서는 `.data` 영역으로 카피가 된다. `.data`영역은 전역 변수나, static 변수가 저장되는 영역이다.
데이터 영역은 변경이 가능하므로, 문자열의 내용을 변경시킬 수 있다.


### 📂 파일 구성


```
3.15-collatz_shard-memory/
├── Makefile
├── collatz.c: 콜라츠 추측 관련 함수 
├── collatz.h
├── error.c	: 커맨드라인 에러 메시지 
├── error.h
└── main.c	
```

## 출처
- [](https://shinluckyarchive.tistory.com/213)
- [data segment - wikipedia](https://en.wikipedia.org/wiki/Data_segment#)
- [콜라츠 추측 - 위키백과](https://ko.wikipedia.org/wiki/%EC%BD%9C%EB%9D%BC%EC%B8%A0_%EC%B6%94%EC%B8%A1)


# \[3.15] 콜라츠 추측(Collatz conjecture) - POSIX Shared Memory

## 📝 문제 

![](https://i.imgur.com/iVyh7Z9.png)

프로그래밍 문제 3.14를 shared memory를 사용하는 프로그램으로 수정하는 문제이다.

다음을 만족하는 C 프로그램을 작성하라.

- 커맨드 라인으로 시작 숫자를 입력 받는다.
- 커맨드 라인으로 받은 숫자가 양의 정수인지 확인하는 에러 체크가 필요하다.
- **자식 프로세스는 콜라츠 추측의 시퀀스를 shared-memory에 작성**한다.
- 부모 프로세스는 자식 프로세스의 실행이 끝났을 때, 그 결과를 출력한다.
- **부모 프로세스는 POSIX shared memory를 만들어야 한다.**

## 👩🏻‍💻 과정

### 1. 부모 프로세스가 shared memory 영역을 만든다.
```c
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
```

- **`shm_open()` 으로 POSIX shared memory object을 만든다**. 

	부모 프로세스는 이 때 flag를 `O_CREAT | O_RDWR`로 하여야 한다. 부모 프로세스는 자식 프로세스가 shared memory에 작성한 것을 읽을 뿐이라서 `O_RDONLY` flag라고 생각했었다. 그러나, `ftruncate()`로 파일의 크기를 지정할 때는 파일이 반드시 쓰기 가능해야 한다. 따라서 `O_RDWR`로 생성해야 한다. 
	```
	With ftruncate(), the file must be open for writing
	```
- **`ftruncate()`로 shared memory object의 사이즈를 지정한다.**

	`shm_open()`의 반환값은 shared memory oject의 file descriptor이다. 이 file descriptor와 원하는 byte size를 인자로 넘겨주어서, object의 크기를 지정한다.

- **`mmap()`으로 shared memory object로 채워진 가상 메모리 영역을 할당한다.**

	`mmap()`은 호출한 프로세스의 가상 메모리 영역에 mapping을 만드는 것이다. 

	첫 번째 인자로 mapping의 시작 주소를 넘겨줄 수 있다. NULL을 넘겨줌으로써, 커널이 알아서 시작 주소부터 length 크기의 메모리 영역을 잡을 수 있게 한다.

	이 메모리 영역의 내용은 shared memory object로 채울 수 있다. object의 file descriptor와 offset을 넘겨주어서, 해당 object로 메모리를 채운다.

	부모 프로세스는 mmaping 영역을 읽기만 할 것이므로, `PROT_READ`로 지정한다.

	생성된 mapping 영역은 자식 프로세스에게도 보여져야 하므로 `MAP_SHARED`로 지정한다.
	
- **생성된 shared memory object는 `/dev/shm`에서 확인할 수 있다.**

	![](https://i.imgur.com/Ww6fcWf.png)

### 2. 자식 프로세스는 mapping 영역에 접근할 fd를 얻는다.
```c
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
```
- **`shm_open()`으로 부모 프로세스가 만든 POSIX shared memory object를 연다.**

- **`mmap()`으로 mapping에 접근할 fd를 얻는다.**

	이 메모리에 값을 쓸 것이므로, `PROT_WRITE`로 쓰기 권한을 갖도록 한다.
	
### 3. 자식 프로세스는 콜라츠 시퀀스를 shared memory에 작성한다.

```c
void* mmapPtr = openSharedMemory(NAME, SIZE);
char buf[SIZE];
bufCollatzSequenceStr(startNum, buf, SIZE);
snprintf(mmapPtr, SIZE, "%s", buf);
```
char array인 buf에 콜라츠 시퀀스를 담아서, mapping 영역의 fd인 mmapPtr로 buf의 내용을 쓴다. 

이 때 `/dev/shm/collatz`를 열어보면 자식 프로세스가 작성한 내용을 확인해볼 수 있다.

![](https://i.imgur.com/9d140Vb.png)


### 4. 부모 프로세스는 shared memory에 있는 내용을 읽는다.

```c
wait(0x0);
printf("%s", (char*)mmapPtr);
```
먼저 자식 프로세스가 shared memory에 쓰고 나서 출력해야 하므로 `wait()`으로 자식 프로세스가 끝나기를 기다린다.

읽기 권한으로 메모리에 접근할 수 있는 fd인 `mmapPtr`로 해당 내용을 읽어와서, 출력한다.

### 5. 부모 프로세스는 shared memory object와 mapping 영역을 지운다.

```c
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
```

- `shm_unlink()`와 `munmap()`으로 shared memory object와 mapping 영역을 지운다.

## ⚙️ 실행 방법

`make`로 컴파일 후, 커맨드 라인으로 양의 정수 하나를 입력한다.

```
$ make
$ ./main {positive_integer}
```

`shm_open()` 시스템 콜을 사용하는 프로그램을 컴파일 할 때, 옵션으로 `-lrt`을 넣어주어야 한다.


## ✔️ 결과 

자식 프로세스에서 생성한 sequence를 부모 프로세스가 Shared memory에서 읽어와서 출력하는 것을 확인할 수 있다.

![](https://i.imgur.com/9AtJuGD.png)




## 💡 What I Learned 

### 1. errno와 strerror로 에러 메시지 출력하기
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

### 2. string을 담은 배열의 주소값을 반환한다?

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

이 문제를 해결하기 위해, 해당 함수를 호출하는 쪽에서 buf 배열을 만들고, 그 주소값을 넘겨주었다. 함수가 리턴되더라도, 함수를 호출한 쪽의 데이터는 메모리에 남아있다.

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


### 3. string의 lifetime

![](https://i.imgur.com/Uw0MNNi.png)

```c=
char* str = "hello\n";
```
char 포인터 변수가 가리키는 "hello\n"는 code 영역(text 영역이라고도 한다)의 `.rodata` 영역에 저장된다. 이 영역은 프로그램이 실행하기 전에 지정되는 영역이다.

code 영역은 stack 영역과는 달리, 프로그램이 종료될 때까지 살아있는 영역이다.

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
여기서 "hello" 또한 마찬가지로 read-only data 영역에 담기지만, 
str 배열이 있는 메모리 영역에 char가 하나씩 카피되어 저장된다.



## 📂 파일 구성


```
3.14-collatz/
├── Makefile
├── collatz.c: 콜라츠 추측 관련 함수 
├── collatz.h
├── error.c	: 커맨드라인 에러 메시지 
├── error.h
└── main.c	
```

## 출처
- [String Constant(문자열 상수)](https://shinluckyarchive.tistory.com/213)
- [data segment - wikipedia](https://en.wikipedia.org/wiki/Data_segment#)
- [콜라츠 추측 - 위키백과](https://ko.wikipedia.org/wiki/%EC%BD%9C%EB%9D%BC%EC%B8%A0_%EC%B6%94%EC%B8%A1)
- [POSIX Shared Memory with C Programming](https://linuxhint.com/posix-shared-memory-c-programming/)
- [What is the difference between char s[] and char *s? - stackoverflow](https://stackoverflow.com/questions/1704407/what-is-the-difference-between-char-s-and-char-s)

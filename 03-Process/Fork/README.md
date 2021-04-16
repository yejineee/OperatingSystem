

# Fork

[hongshin/OperatingSystem](https://github.com/hongshin/OperatingSystem) 레포의 sysprog 브랜치에서 가져온 Fork 예제입니다.

## 🔗 Contents

- [실행 방법](https://github.com/yejineee/OperatingSystem/new/master/03-Process/Fork#%EF%B8%8F-%EC%8B%A4%ED%96%89-%EB%B0%A9%EB%B2%95)
- [Fork 예제에서 사용한 system call 알아보기](https://github.com/yejineee/OperatingSystem/new/master/03-Process/Fork#%EF%B8%8F-fork-%EC%98%88%EC%A0%9C%EC%97%90%EC%84%9C-%EC%82%AC%EC%9A%A9%ED%95%9C-system-call-%EC%95%8C%EC%95%84%EB%B3%B4%EA%B8%B0)
- [Fork0.c 설명](https://github.com/yejineee/OperatingSystem/new/master/03-Process/Fork#-fork0c-%EC%84%A4%EB%AA%85)

## ⚙️ 실행 방법

`make`로 컴파일한 후, 원하는 실행파일을 실행시킨다. 

## ✏️ Fork 예제에서 사용한 system call 알아보기

### fork()

```c
#include <sys/types.h>
#include <unistd.h>

pid_t fork(void);
```

fork() 시스템 콜은 이 함수를 호출한 프로세스를 복제하여 자식 프로세스를 생성한다.

이 때 함수를 호출한 프로세스를 부모 프로세스, 생성된 프로세스를 자식 프로세스라고 한다.

부모 프로세스와 자식 프로세스는 서로 다른 메모리 영역에서 동작하게 된다.

성공적으로 프로세스가 생성되면, 부모 프로세스에게는 자식 프로세스의 PID가 반환되고, 자식 프로세스는 0이 반환된다.

실패하면, 호출한 프로세스에게 -1이 반환되며, erron가 설정된다.

### wait()

```c
#include <sys/types.h>
#include <sys/wait.h>

pid_t wait(int *wstatus);
```
부모 프로세스가 자식 프로세스의 state가 변경되는 것을 기다릴 떄 사용된다.

state가 변경된다는 것은 자식 프로세스가 종료되거나, 자식 프로세스가 signal에 의해 멈춰지거나, signal에 의해 다시 시작하게 될 때를 말한다.

wait()의 반환값은 성공적이었을 때, 종료된 프로세스의 PID이고, 에러가 발생하면 -1이다.

### execl()

```c
#include <unistd.h>

extern char **environ;

int execl(const char *pathname, const char *arg, ...
			   /* (char  *) NULL */);
```

메모리에 있는 현재 프로세스 image를 새로운 프로세스의 image로 대체시킨다.

실행시킬 파일의 path와 그 파일의 인자들을 나열한다. 

arg0에는 컨벤션으로 실행 파일의 이름이 들어간다.

마지막 인자에는 반드시 null pointer가 들어가야 한다. 

그리고 이 null pointer는 반드시 casting이 되어야 하는데, `(char*) NULL`로 하면 된다.




	
	

### getpid()

```c
#include <sys/types.h>
#include <unistd.h>

pid_t getpid(void);
pid_t getppid(void);
```

getpid()는 이 함수를 호출한 프로세스의 pid를 반환한다.

getppid()는 이 함수를 호출한 프로세스의 부모 프로세스의 PID를 반환한다.


## 📝 Fork0.c 설명

- **코드** 

	```c
	#include <stdio.h>
	#include <stdlib.h>
	#include <unistd.h>
	#include <sys/types.h>
	#include <sys/wait.h>

	int
	main ()
	{
		pid_t child_pid ;
		int data = 0 ;

		child_pid = fork() ;
		if (child_pid == 0) { // child process
			printf("Child: %d\n", data) ;
		}
		else if (child_pid > 0) { // parent process
			data = 1 ;
			printf("Parent: %d\n", data) ;
			wait(0x0) ;
		}
		else /* child_pid < 0 */ {
			printf("Fork failed.\n") ;
		}
	}
	```

- **실행 결과**
	
	```
	Parent: 1
	Child: 0
	```

- **실행순서는 어떻게 되는가?** 

	부모 프로세스가 먼저 실행된 후, 자식 프로세스가 실행되었다. 

	fork()로 부모 프로세스를 clone하여 자식 프로세스가 만들어지게 된다.
	
	새로운 프로세스가 만들어지는데 걸리는 Overhead가 있고, 자식 프로세스가 레디큐에 있다가 스케쥴되어 프로세서가 실행시키기까지의 시간이 걸린다. 
	
	**그러나, 이 순서는 고정된 것이 아니다.**
	
	스케쥴링이 어떻게 되냐에 따라 자식 프로세스가 먼저 실행될 수 있다.
	
- **두 프로세스의 메모리 영역이 다르다는 것**

	실행 결과를 보면, 부모에서 먼저 data 변수를 출력을 하고, 자식 프로세스가 data를 출력한다. 
	
	부모가 data를 출력하기 전에 값을 1로 변경시킨다. 
	
	그러나 자식 프로세스가 data를 출력하였을 때, 그 값은 1이 아닌 0이다.
	
	왜 이런일이 생긴걸까?
	
	부모 프로세스가 자식 프로세스를 fork()하면, 자식 프로세스는 **fork()했을 당시의 부모 프로세스의 메모리를 다른 메모리 영역에 복사**하게 된다.
	
	따라서 자식 프로세스 또한 data라는 변수를 갖지만, 다른 메모리 영역에 있기 때문에 부모 프로세스에서 값이 변경되었어도, 자식 프로세스는 여전히 값이 0인 것이다.
	
	
	
	


## \[3.14] 콜라츠 추측(Collatz conjecture)

### 📝 문제 

![](https://i.imgur.com/iVyh7Z9.png)

콜라츠 추측은 위 알고리즘을 계속해서 적용하면 모든 양의 정수는 결국 1이 된다는 것이다.

다음을 만족하는 C program을 작성하라.

- 커맨드 라인으로 시작 숫자를 입력 받는다.
- 부모 프로세스는 `fork()`로 자식 프로세스를 만들고, 자식 프로세스가 이 sequence를 만든다.
- 자식 프로세스가 결과를 출력한다.
- 부모 프로세스는 자식 프로세스가 끝나기를 `wait()`으로 기다린 후, 자식 프로세스가 종료되면 프로그램을 종료시킨다. 
- 커맨드 라인으로 받은 숫자가 양의 정수인지 확인하는 에러 체크가 필요하다.

### ⚙️ 실행 방법

`make`로 컴파일 후, 커맨드 라인으로 양의 정수 하나를 입력한다.

```
$ make
$ ./main {positive_integer}
```

### 📂 파일 구성

컴파일을 간편하게 하고, 실수를 줄이고자 Makefile을 생성하였다.

에러 메시지는 파일로 따로 분리하여, 관리에 용이하게 하였다.

콜라츠 추측 관련된 함수 또한 따로 분리하였다. 

기능별로 파일을 나눠서 가독성을 높이고, 이후 발전될 프로젝트에서 수정하기 용이하게 하였다.


```
3.14-collatz/
├── Makefile
├── collatz.c: 콜라츠 추측 관련 함수 
├── collatz.h
├── error.c	: 커맨드라인 에러 메시지 
├── error.h
└── main.c	
```

### ✔️ 결과 

- 양의 정수를 입력하지 않은 경우의 에러 핸들링
	![](https://i.imgur.com/BPDegdZ.png)

- 양의 정수의 경우 콜라츠 추측의 시퀀스 출력
	![](https://i.imgur.com/C4kIto9.png)

	커맨드 라인으로 적절한 값이 들어온 경우, 자식 프로세스가 1이 될 때까지 콜라츠 추측의 시퀀스를 출력한다.
	
	자식 프로세스가 생성되었을 때, 부모 프로세스와는 다른 영역이지만, 같은 메모리의 내용을 복제하였으므로, 사용자가 입력한 startNum에 적절한 값이 있다. 
	
	이 값을 `printCollatzSequence()` 함수에 넘겨주어서, 자식 프로세스의 역할을 수행한다.

	```c
	int startNum = atoi(argv[1]);
	pid_t childPid ;

	childPid = fork() ;

	if (childPid == 0) { // child process
		printCollatzSequence(startNum);
	}
	```



## 출처
- [콜라츠 추측 - 위키백과](https://ko.wikipedia.org/wiki/%EC%BD%9C%EB%9D%BC%EC%B8%A0_%EC%B6%94%EC%B8%A1)

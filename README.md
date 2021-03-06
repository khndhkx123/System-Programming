# System-Programming
Advanced Project in the UNIX Environment


## [시그널(Signal) Submission]

### (stop.c) Ctrl-C 는 실행중인 프로그램을 종료시키는 방법이다.
> Ctrl-C를 누르면 키보드가 현재 실행중인 프로세스에게 시그널을 보낸다.
시그널을 받은 프로세스는 종료된다. Ctrl-C를 눌렀을 때, 
“Ctrl-C is pressed. Try Again.”이라는 문장을 화면에 출력하고 실행을 계속하도록 하는 프로그램을 작성하시오.
Ctrl-C를 눌렀을 때, 발생하는 시그널의 번호를 파악하시오.
시그널이 발생했을 때, "Ctrl-C is pressed. Try Again" 이라는 문장을 출력하는 시그널 핸들러를 작성하시오.
Ctrl-C를 눌렀을 때, 위에서 작성한 시그널 핸들러가 실행되도록, 시그널 핸들러를 재설정하는 프로그램을 작성하시오.

### (count.c) 시그널을 이용해서 프로세스의 작동을 동기화 하는 프로그램을 다음과 같이 작성한다.
> count.c는 0보다 큰 정수를 인자로 받는다. 세개의 프로세스 예를 들어 P1, P2, P3를 생성한다.
생성된 프로세스들은 돌아가면서 파일에 저장되어 있는 정수값을 1씩 증가시킨다.
파일 sample.txt에 0 이라는 문자가 저장되어 있다. P1, P2, P3 각각은 sample.txt에 있는 값을 읽어, 값을 1 증가시키고, 다시 파일에 저장한다.
각 프로세스는 sleep상태에 있다. 갱신순서는 P1 -> P2 -> P3 ->P1 ...의 순서로 돌아가면서 실행된다.
하나의 프로세스가 갱신이 끝나면 자신은 sleep상태로 들어가고 다음 프로세스를 깨운다.
정수 값 0이 저장되어 있는 파일을 생성한다.
시그널 전송은 kill(pid, signal 번호) 함수를 사용하시오.
사용 예: sample.txt에 0이 저장되어 있을 경우, 이 값을 300까지 증가시킨다.
              $ count 300 sample.txt
              
> 테스트할 사항 : 
첫번째 인자가 0보다 큰 정수인지를 확인한다.
두번째 인자에 해당하는 파일(sample.txt)이 존재하는지 확인하고, 없으면 생성한다.
sample.txt 파일이 존재할 경우, 첫줄에 0이 있는지를 확인한다. 파일의 내용이 다를 경우  파일을 truncate해서 0을 저장한 후에 프로그램을 시작한다.
출력물:
              $ count 100000 sample.txt
의 실행 시간을 출력하시오.

### mysignaltest.c 라는 파일을 참고하시오. mysignaltest.c파일을 컴파일하여 실행한다.

> Ctrl-C를 눌러 프로그램 실행을 중단한다. 
화면에 나오는 프로그램의 출력물을 확인한다.
프로그램이 제대로 실행되는지 확인하고, 제대로 실행되지 않는다면 그 이유를 설명하시오.
시그널 핸들러가 제대로 작동하도록 수정하시오.





## [쓰레드(Thread) Submission]

### 4000 X 4000 의 정수 행렬 2개를 곱하는 함수를 작성하시오.
> multi(A, B, C) -> C = A*B

### 행렬은 파일에 저장되어 있다. 프로그램 수행 시작할 때, 파일에 있는 행렬을 읽어들여 행렬에 저장한다.
> 1. 두 행렬의 곱을 계산한 후, 결과 행렬의 모든 원소를 더하는 연산을 실행하시오. 시간을 측정하시오. 시간은 곱셈 연산 시작 직후부터 계산값을 모두 더하기까지에 소요된 시간을 구한다.
> 2. 쓰레드를 이용하여 행렬의 곱을 병렬화하고자 한다. 두개의 쓰레드를 생성한다. 하나의 쓰레드는 A행렬의 상위 절반의 행을 할당 받는다. 또하나의 쓰레드는 A행렬의 하위 절반의 행을 할당받는다. 이를 각각 A1, A2라 하자. 두개의 쓰레드가 각각 A1*B, A2*B를 실행한다. 결과행렬을 C1 = A1*B, C2 = A2*B라 하자.  C1과 C2를 사용하여 결과행렬 C를 생성한다. 결과행렬 원소의 총합을 구한다. 1번에서 실행했던 연산을 실행하시오. 시간을 측정하시오. 연산 시간이 개선되었는가, 개선되었다면 그 원인은 무엇인지 논하시오.
> 3. N개의 쓰레드를 이용하여 곱셈연산을 실행하는 프로그램을 작성하시오. A행렬을 N개의 서브행렬로 분할한다. A1, A2, … AN. N개의 쓰레드를 생성하여, 각 쓰레드가 Ci = Ai * B의 연산을 수행한다. N = 1,2,4,6,8,10,20,40 개의 쓰레드를 이용하여 실행한다. 1번에서와 같이 곱셈을 수행한 후에, 결과 행렬을 모두 더한다. 결과값을 모두 더하는 것은 각 쓰레드가 할당된 곱셈을 모두 마친후에 시작한다. 각 쓰레드에 할당된 곱셈연산을 모두 마칠때 까지 대기하는데 두가지 방법이 있다. pthread_join을 이용하거나, pthread_barrier를 사용할 수 있다. 두가지 방법을 모두 사용하여 수행한다.
쓰레드의 갯수를 증가에 따른 연산시간 단축에 대해, 아래의 그래프를 그리시오.
X축: 쓰레드의 개수, N = 1, 2, 4, 8, 10, 20, 30, 40
Y축: 해당 쓰레드 개수에 대한 실행 시간 / (1개의 쓰레드로 수행한 실행시간)

### 위의 기법에서는 모든 쓰레드가 연산을 종료한 후, 합산을 시작한다. 
### 각 쓰레드가 행렬 곱셈을 수행할때, 곱셈을 수행한 결과 행렬의 합을 미리 구해서, 합산을 시작한다면 총 연산시간을 좀더 단축 시킬 수 있다.
### 해당 기법을 구현해서 실제 효과를 보이도록 하시오. 다중 쓰레드를 이용하여 실행속도를 최대한 줄이도록 하시오. 최대한 얼마까지 줄였는지, 본인의 개선 방법에 대해 설명하시오.


## [락(Lock) Submission]

### 락 획득 대기 중, 일정 시간 동안은 spin하고, 일정 시간 후에는 쓰레드를 블락시키는 락을 구현한다. spin lock, mutex 등을 적절히 이용하여 Hybrid Lock 을 구현한다. 자세한 사항은 다음과 같다.

> 1. 락 획득 대기 시, 1초 동안 spin하며, 락을 대기한다.
1초가 지나도 락을 획득하지 못한다면 해당 쓰레드를 블락시킨다.
락은 블락 상태에 있는 쓰레드에게 우선적으로 넘겨준다.
구현해야할 락 인터페이스 (hybrid_lock.h, hybrid_lock.c)
struct hybrid_lock: 새로 구현한 락을 나타내는 자료구조
hybrid_lock_init(): 락을 초기화하는 함수
hybrid_lock_destroy(): 락을 사용이 끝났을 때, 메모리에서 해제해주는 함수
hybrid_lock_lock(): 락을 점유하는 함수
hybrid_lock_unlock(): 락을 해제하는 함수

> 2. test1.c와 test2.c는  두 개의 인자를 받는다. 첫번째 인자는 생성할 쓰레드의 숫자고, 두번째 인자는 생성된 쓰레드가 전역변수 g_count를 increase할 숫자이다. 두 프로그램은 락에 의해 보호되어야 하는 critical section을 갖고 있다. 각각의 critical section은 현재 주석으로 표기되어 있는 부분이다. 이 때, test1과 test2 프로그램에서 critical section을 mutex, spin lock, hybrid lock으로 각각 보호했을 때의 성능차를 구하시오. test1은 10개의 쓰레드가  10,000,000번 increase할 때의 성능을 비교하고, test2는 4개의 쓰레드가 10번 increase할 때의 성능을 비교하시오.
제출 항목
각각의 프로그램에서 mutex, spin lock, hybrid lock을 사용했을 때의 runtime을 비교한 그래프를 보고서에 포함
각 프로그램에서 각 락을 사용했을 때, 성능이 달라지는 이유를 보고서에 작성

> 3. 쓰레드를 일정 시간동안 spin 시키는 방법에는 여러가지가 있을 수 있다. 대표적인 방법은 while()문을 1초동안 실행시키는 것이다. 1초가 지난 것을 확인하기 위해서는 while() 문 안에서 gettimeofday()와 같은 시스템 콜로 시간을 확인해야 한다. 하지만, gettimeofday()를 모든 while() 루프마다 실행하는 것은 비효율적이다. 이를 효율적으로 바꾸려면, while() 루프 하나가 어느 정도의 시간을 소모하는지 계산한 후, 1초 동안 실행되는 while() 루프 횟수를 구하고, 해당 횟수만틈 while() 루프를 실행했을 때만 gettimeofday()로 시간을 측정해야 한다. 이 효율성은 (gettimeofday() 를 호출한 횟수 * (spin한 총 시간 - 1초))로 나타낼 수 있다. 자신이 개발한 hybrid lock에서 이 수치가 어떻게 나오는지 보고서에 추가하시오.



## [IPC 과제]
### Shared memory와 Message Queue를 통한 다중 채팅 & 메신져 프로그램을 구현한다. 자세한 사항은 다음과 같다.

### IPC 구성 요소
> Server 프로세스(1개)
Client 프로세스(3개)

### 채팅 내용을 기록하기 위한 Shared Memory: Chat-log Memory (1개)
> Shared Memory인 Chat-log Memory에는 Chat-Queue를 확인하여, 메시지를 보낸 프로세스의 Process ID(PID)와 메시지의 내용을 저장하여야 하며, 최대 100개까지 채팅 내역을 보관하여야 한다. 이를 위한 자료구조는 자유롭게 정의하여도 된다. 
Shared Memory를 위한 Key 값은 Server와 Client 프로세스 모두 사전에 정의하도록 한다.

### Client 프로세스의 채팅 메시지를 위한 Message Queue: Chat Queue (3개: Client 프로세스 당 1개)
> Chat Queue는 Client 프로세스에서 보낸 메시지 내용을 가지고 있어야 하며, 특정 PID에게 메시지를 보낼 것인지, 혹은 전체 채팅창에 메시지를 보낼 것인지 결정할 수 있어야 한다. 한번에 보낼 수 있는 채팅 메시지의 크기는 1024byte로 제한한다. 메시지 큐를 위한 Key 값은 Server와 Client 프로세스 모두 사전에 정의하도록 한다.

### Server 프로세스 동작
> 프로세스 별 Chat Queue들을 검사하여 해당 Chat Queue에 메시지의 내용을 확인한다.
메시지가 만약 특정 PID에게 보내지는 것이라면, Chat-log Memory에 저장하지 않고 해당 PID의 Chat-Queue에 메시지를 전송한다. 메시지가 특정 PID에게 보내지지 않았다면, 메시지를 보낸 프로세스의 PID와 메시지 내용을 조합하여 Chat-log Memory에 저장한 후 이를 아래와 같이 출력한다.

### Client 프로세스 동작
> Client 프로세스는 사용자 입력에 따라 다음과 같은 작업을 진행할 수 있다.
나에게 온 메시지를 확인할 수 있다. 받은 메시지는 아래와 같이 확인할 수 있다.
특정 프로세스(PID)에게 메시지를 전송할 수 있다.
현재 채팅 로그를 확인할 수 있다.

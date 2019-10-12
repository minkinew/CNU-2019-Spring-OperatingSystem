#include <stdio.h>
#include <pthread.h>

void ping(void);
void pong(void);

int number=1;
int pingpong=1; // ping과  pong의 처음 순서를 정해주는 flag 값
pthread_mutex_t mutex;

int main()
{
	pthread_t process1, process2;
	pthread_mutex_init(&mutex, NULL); //Mutex 생성

	//thread 생성
	pthread_create(&process1, NULL, (void*)&ping, NULL);	
	pthread_create(&process2, NULL, (void*)&pong, NULL);	
	
	//thread 동작
	pthread_join(process1, NULL);
	pthread_join(process2, NULL);

	pthread_mutex_destroy(&mutex); //Mutex 제거
}

void ping(void)
{
	int i;
	///mutex 및 printf에서 출력하는 관련 변수 코드 채우기
	for (i=0; i<50; i++)
	{
		pthread_mutex_lock(&mutex); //잠금을 생성
		if( pingpong )
		{
			printf("%4d - %1d - ping\n", number, i);
			number++;
			pingpong = 0;
		}
		else
		{
			i--;
		}
		pthread_mutex_unlock(&mutex); //잠금을 해제
	}
	pthread_exit(0);
}

void pong(void)
{
	int i;
	//mutex 및 printf에서 출력하는 관련 변수 코드 채우기
	for (i=0; i<50; i++)
	{
		pthread_mutex_lock(&mutex); //잠금을 생성
		if(!pingpong)
		{
			printf("%4d - %1d - pong\n", number, i);
			number++;
			pingpong = 1;
		}
		else
		{
			i--;
		}
		pthread_mutex_unlock(&mutex); //잠금을 해제
	}
	pthread_exit(0);
}

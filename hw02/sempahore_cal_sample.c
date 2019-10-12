#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

int count=0;
int counter=0;

sem_t semaphore; //세마포어 생성

void *add1(void *data){

	int tmp;
	int i =0;
	sem_wait(&semaphore);
	while(i<5){
		i++;
		count++;
		printf("add1 count= %d\n",count);
		tmp=counter;
                sleep(1);
		tmp=tmp+1;
	}
	sem_post(&semaphore);
}

void *add2(void *data){

	int tmp;
        int i =0;
	sem_wait(&semaphore);
	while(i<5){
		i++;
		count++;
		printf("add2 count= %d\n",count);
		tmp=counter;
		sleep(1);
		tmp=tmp+1;
	}
	sem_post(&semaphore);
}

void *add3(void *data){

	int tmp;
        int i =0;
	sem_wait(&semaphore);
	while(i<5){
		i++;
		count++;
		printf("add3 count= %d\n",count);
		tmp=counter;
		sleep(1);
		tmp=tmp+1;
	}
	sem_post(&semaphore);
}

void *sub1(void *data){

	int tmp;
        int i =0;
	sem_wait(&semaphore);
	while(i<5){
		i++;
		count--;
		printf("sub1 count= %d \n",count);
		tmp=counter;
		sleep(1);
		tmp=tmp-1;
	}
	sem_post(&semaphore);
}

void *sub2(void *data){
	
	int tmp;
	int i =0;
	sem_wait(&semaphore);
	while(i<5){
		i++;
		count--;	
		printf("sub2 count= %d \n",count);
		tmp=counter;
		sleep(1);
		tmp=tmp-1;
	}
	sem_post(&semaphore);
}



void *sub3(void *data){
	int tmp;
        int i =0;
	sem_wait(&semaphore);
	while(i<5){ 
		i++;	
		count--;
		printf("sub3 count= %d \n",count);
		tmp=counter;
		sleep(1);
		tmp=tmp-1;
	}
	sem_post(&semaphore);
}

int main(){
	int i=0;
	
	sem_init(&semaphore, 0, 1); //세마포어 할당

	pthread_t pthread[5];
	pthread_create(&pthread[0],NULL,add1,NULL);
	pthread_create(&pthread[1],NULL,sub1,NULL);
	pthread_create(&pthread[2],NULL,add2,NULL);
	pthread_create(&pthread[3],NULL,sub2,NULL);
	pthread_create(&pthread[4],NULL,add3,NULL);
	pthread_create(&pthread[5],NULL,sub3,NULL);


	for(i=0;i<6;i++)
		pthread_join(pthread[i],NULL);

	sem_destroy(&semaphore); //세마포어 반환
	return 0;

}

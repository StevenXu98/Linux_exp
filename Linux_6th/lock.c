#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <pwd.h>
#include <string.h>
#include <time.h>
#include <sys/wait.h>
#include <pthread.h>

pthread_mutex_t mutex[5];

void* philosopher(void* arg)
{
		
		int *number;
		number=(int*) arg;
		while(1)
		{
			int flag = 0;
			sleep(1);
			printf("%d is thinking\n",*number );
			pthread_mutex_lock(&mutex[*number]);
			flag = pthread_mutex_trylock(&mutex[(*number+1)%5]);
			if(flag){
				pthread_mutex_unlock(&mutex[(*number+1)%5]);
				sleep(1);
				pthread_mutex_lock(&mutex[(*number+1)%5]);
				printf("%d is eating\n",*number );
				sleep(1);
				pthread_mutex_unlock(&mutex[*number]);
				pthread_mutex_unlock(&mutex[(*number+1)%5]);
			}
			printf("%d is eating\n",*number );
			sleep(1);
			pthread_mutex_unlock(&mutex[*number]);
			pthread_mutex_unlock(&mutex[(*number+1)%5]);
		}
}

int main(int argc, char const *argv[])
{
	pthread_mutex_t mutex;
	pthread_mutex_init(&mutex,NULL);

	pthread_t tid1,tid2,tid3,tid4,tid5;
	int test1 = 0;  
    int *attr1 = &test1;
    int test2 = 1;  
    int *attr2 = &test2;
    int test3 = 2;  
    int *attr3 = &test3;
    int test4 = 3;  
    int *attr4 = &test4;
    int test5 = 4;  
    int *attr5 = &test5;

	pthread_create(&tid1,NULL,(void*)philosopher,(void*)attr1);
	pthread_create(&tid2,NULL,(void*)philosopher,(void*)attr2);
	pthread_create(&tid3,NULL,(void*)philosopher,(void*)attr3);
	pthread_create(&tid4,NULL,(void*)philosopher,(void*)attr4);
	pthread_create(&tid5,NULL,(void*)philosopher,(void*)attr5);

	pthread_join(tid1,NULL);
	pthread_join(tid2,NULL);
	pthread_join(tid3,NULL);
	pthread_join(tid4,NULL);
	pthread_join(tid5,NULL);

	return 0;
}



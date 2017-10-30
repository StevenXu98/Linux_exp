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
			int result=0;
			sleep(1);
			printf("philosopher %d is thinking\n",*number );
			sleep(1);
			pthread_mutex_lock(&mutex[*number]);
			//printf("philosopher %d is waiting for eating(handle a chopstick)\n",*number );
			result=pthread_mutex_trylock(&mutex[(*number+1)%5]);
			if(result!=0)
			{
					pthread_mutex_unlock(&mutex[(*number+1)%5]);
					sleep(2);
					pthread_mutex_lock(&mutex[(*number+1)%5]);
					printf("philosopher %d is eating\n",*number );
					sleep(2);
					pthread_mutex_unlock(&mutex[*number]);
					pthread_mutex_unlock(&mutex[(*number+1)%5]);
					continue;
			}
			printf("philosopher %d is eating\n",*number );
			sleep(2);
			pthread_mutex_unlock(&mutex[*number]);
			pthread_mutex_unlock(&mutex[(*number+1)%5]);
		}
}

int pthread_mutex_init(pthread_mutex_t *mutex,const pthread_mutexattr_t *attr);
int pthread_mutex_destroy(pthread_mutex_t *mutex);
int pthread_mutex_lock(pthread_mutex_t *mutex);
int pthread_mutex_unlock(pthread_mutex_t *mutex);
int pthread_mutex_trylock(pthread_mutex_t *mutex);
int pthread_create(pthread_t *restrict tidp,
				const pthread_attr_t *restrict attr,
				void *(*start_rtn)(void *), 
				void *restrict arg);
int pthread_join(pthread_t thread,void **rval_ptr);

/*void* dinner(void)
{
	while(1)
	{
		for(int i=1;i<=5;i++)
		{
			philosopher(i);
		}
	}
}
*/
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



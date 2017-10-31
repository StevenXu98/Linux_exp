#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/shm.h>
#include "shmdata.h"
#include <sys/sem.h>
#include <string.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/ipc.h>

union semun {
    int val;
    struct semid_ds *buf;
    unsigned short *arry;
};

static int sem_id = 0;

static int set_semvalue()
{
    //用于初始化信号量，在使用信号量前必须这样做
    union semun sem_union;

    sem_union.val = 1;
    if (semctl(sem_id, 0, SETVAL, sem_union) == -1)
        return 0;
    return 1;
}

static int sem_p(int semid)
{
    struct sembuf sem_arg;
    sem_arg.sem_num = 0;
    sem_arg.sem_op = -1;
    sem_arg.sem_flg = SEM_UNDO;

    if (semop(semid, &sem_arg, 1) == -1)
    {
        printf("%s : can't do the sem_p!\n", __func__);
        return -1;
    }
    return 0;
}

static int sem_v(int semid)
{
    struct sembuf sem_arg;
    sem_arg.sem_num = 0;
    sem_arg.sem_op = 1;
    sem_arg.sem_flg = SEM_UNDO;

    if (semop(semid, &sem_arg, 1) == -1)
    {
        printf("%s : can't do the sem_v!\n", __func__);
        return -1;
    }
    return 0;
}

static int del_sem(int semid)
{
    if (semctl(semid, 0, IPC_RMID) == -1)
    {
        printf("%s : can't rm the sem!\n", __func__);
        return -1;
    }
    return 0;
}

int main()
{
    int running = 1;
    void *shm = NULL;
    struct shared_use_st *shared = NULL;
    char buffer[BUFSIZ + 1]; //用于保存输入的文本
    int shmid;

    sem_id = semget((key_t)1234, 1, 0666 | IPC_CREAT);
    if(!set_semvalue()){
        fprintf(stderr,"Failed to initialize semaphore\n");
        exit(EXIT_FAILURE);
    }

    //创建共享内存
    shmid = shmget((key_t)1234, sizeof(struct shared_use_st), 0666 | IPC_CREAT);
    if (shmid == -1)
    {
        fprintf(stderr, "shmget failed\n");
        exit(EXIT_FAILURE);
    }
    //将共享内存连接到当前进程的地址空间
    shm = shmat(shmid, (void *)0, 0);
    if (shm == (void *)-1)
    {
        fprintf(stderr, "shmat failed\n");
        exit(EXIT_FAILURE);
    }
    printf("Memory attached at %X\n", (int)shm);
    //设置共享内存
    shared = (struct shared_use_st *)shm;
    while (running) //向共享内存中写数据
    {

        //数据还没有被读取，则等待数据被读取,不能向共享内存中写入文本
        while (shared->written == 1)
        {
            sleep(1);
            printf("Waiting...\n");
        }

        if(sem_p(sem_id)){
            printf("%s : sem_p failed! \n",__func__);
            return -1;
        }

        //向共享内存中写入数据
        printf("Enter some text: ");
        fgets(buffer, BUFSIZ, stdin);
        strncpy(shared->text, buffer, TEXT_SZ);

        if (sem_v(sem_id))
        {
            printf("%s : sem_v failed!\n", __func__);
            return -1;
        }
        //写完数据，设置written使共享内存段可读
        shared->written = 1;
        //输入了end，退出循环（程序）
        if (strncmp(buffer, "end", 3) == 0)
            running = 0;
        
            sleep(2);
    }
    /*删除信号量*/

    //把共享内存从当前进程中分离
    if (shmdt(shm) == -1)
    {
        fprintf(stderr, "shmdt failed\n");
        exit(EXIT_FAILURE);
    }
    sleep(2);
    exit(EXIT_SUCCESS);
}
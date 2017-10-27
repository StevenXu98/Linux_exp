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

#define FILE_MODE 0777
#define DIR_MODE 0777

int fd[2] = {0};

int main(int argc ,char ** argv)
{
    struct stat get_message;
    char buffer[100];
    struct dirent *drip;
    

    mkdir("/home/xu/xuyiteng",DIR_MODE);
    char *getcwd(char *buf, size_t size);
    if(getcwd(buffer,100)){
        printf("%s\n",buffer);
    }

    char *source = buffer;
    char *destinatuion = "/home/xu/xuyiteng";

    while ((dirp = readdir(dp)) != NULL){
        if(strcmp(drip->d_name,"..") && strcmp(drip->d_name,".")){
            
        }
    }
    
    return 0;
}
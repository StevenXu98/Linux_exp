#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>


/*复制文件时的读写缓冲区大小*/
#define BUF_SIZE 8092
/*保存文件路径的缓冲区大小*/
#define PATH_SIZE 1024
/*文件权限*/
#define FILE_MODE 0777
/*目录权限*/
#define DIR_MODE 0777

int fd[2] = {0};

void deal_dir(const char*, const char*);
void deal_copy(const char*,const char*);
void get_allpath(char *,char *, const char *,const char *);
void init_allpath(char *,char *,char *,char *,const char *);
void mkdir_newdir(const char *);
void read_write(void);



void deal_dir(const char* old_path, const char* new_path)
{
    DIR* ret_opdir = opendir(old_path);
    struct dirent* ret_redir;
    char buf_old[PATH_SIZE] = {};
    char buf_new[PATH_SIZE] = {};
    char buf_oldfile[PATH_SIZE] = {};
    char buf_newfile[PATH_SIZE] = {};
    get_allpath(buf_old, buf_new, old_path, new_path);

    while((ret_redir = readdir(ret_opdir))){
        if(!strcmp(ret_redir->d_name, ".")||!strcmp(ret_redir->d_name,".."))
        continue;
        else{
            init_allpath(buf_old, buf_new, buf_oldfile, buf_newfile, ret_redir->d_name);
            if((ret_redir->d_type & DT_REG) == DT_REG)
            deal_copy(buf_oldfile,buf_newfile);
            else if((ret_redir->d_type & DT_DIR) == DT_DIR)
            deal_dir(buf_oldfile,buf_newfile);
        }
    }
}

void get_allpath(char* buf_old, char* buf_new, const char* old_path, const char* new_path)
{
    char buf[PATH_SIZE] = {};
    getcwd(buf,PATH_SIZE);
    chdir(old_path);
    getcwd(buf_old,PATH_SIZE);
    strcat(buf_old,"/");

    chdir(buf);
    mkdir_newdir(new_path);
    chdir(new_path);
    getcwd(buf_new,PATH_SIZE);
    strcat(buf_new,"/");

}

void init_allpath(char* buf_old, char* buf_new, char* buf_oldfile, char* buf_newfile, const char* name)
{
    memset(buf_oldfile, 0, PATH_SIZE);
    memset(buf_newfile, 0, PATH_SIZE);

    strcpy(buf_oldfile, buf_old);
    strcat(buf_oldfile, name);

    strcpy(buf_newfile, buf_new);
    strcat(buf_newfile, name);
}

void mkdir_newdir(const char * new_path)
{
    /*目录不存在返回NULL，则创建空目录*/
    DIR * ret_opdir = opendir(new_path);
    if(ret_opdir == NULL){
        int ret_mkdir = mkdir(new_path, DIR_MODE);/*创建子目录*/
    }
}

void deal_copy(const char *old_file,const  char * pathname)
{
    struct stat get_message;
    int ret_stat = stat(pathname, &get_message);
    fd[1] = open(pathname, O_CREAT | O_TRUNC | O_RDWR, FILE_MODE);  

    fd[0] = open(old_file, O_RDONLY);

    read_write();

    close(fd[0]);
    close(fd[1]);
}   
void read_write(void)
{
    char buf[BUF_SIZE] = {};
    int ret_read, ret_write;
    while((ret_read = read(fd[0], buf, BUF_SIZE)) > 0){
        ret_write = write(fd[1], buf, strlen(buf));
    }
}

int main(int argc, char** argv)
{
    if(argc != 3){
        printf("too few or less parameter! \n");
        exit(-1);
    }else{
        struct stat get_message;
        
        if(S_ISDIR(get_message.st_mode))
            deal_dir(argv[2],argv[3]);
        else{
            if(!strcmp(argv[2],argv[3])){
                printf("same filename error");
                exit(-1);
            }
            deal_copy(argv[2],argv[3]);
        }
    }
    return 0;
}

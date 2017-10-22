#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

/*复制文件时的读写缓冲区大小*/
#define BUF_SIZE 8092
/*保存文件路径的缓冲区大小*/
#define PATH_SIZE 1024
/*文件权限*/
#define FILE_MODE 0777
/*目录权限*/
#define DIR_MODE 0777

void sys_err(const char *); /*errno错误处理*/
void deal_dir(const char *,const char *);   /*递归函数，处理目录*/
void deal_copy(const char *,const char *);  /*处理绝对路径的文件*/
void read_write(void);  /*拷贝文件时的读写操作*/
void mkdir_newdir(const char *);    /*为目标创建空目录*/
void get_allpath(char *,char *,const char *,const char *);  /*获取source与target的绝对路径*/
void init_allpath(char *,char *,char *,char *,const char *);    /*清空存储路径与文件名的缓冲区并重新赋值*/

int fd[2] = {0};

void sys_err(const char * ptr)
{
    perror(ptr);
    exit(EXIT_FAILURE);
}

void deal_dir(const char *old_path,const char * new_path)
{
    DIR * ret_opdir = opendir(old_path);//打开目录
    if(ret_opdir == NULL)
        sys_err("opendir");
    struct dirent * ret_redir;

    /*定义缓冲区存放绝对路径，eg:/home/krj/test/*/
    char buf_old[PATH_SIZE] = {};
    char buf_new[PATH_SIZE] = {};
    /*存放绝对路径+文件名,eg:/home/krj/test/test.txt*/
    char buf_oldfile[PATH_SIZE] = {};
    char buf_newfile[PATH_SIZE] = {};

    /*获取源文件完整路径与目标位置完整路径,分别存入buf_old与buf_new中*/
    get_allpath(buf_old, buf_new, old_path, new_path);  

    while((ret_redir = readdir(ret_opdir))){//读取目录，失败或目录读完返回NULL
        if(!strcmp(ret_redir->d_name, ".") || !strcmp(ret_redir->d_name, ".."))
            continue;
        else{
            /*清除上次复制的文件路径，并填入新读取到的文件的路径*/
            init_allpath(buf_old, buf_new, buf_oldfile, buf_newfile, ret_redir->d_name);
            if((ret_redir->d_type & DT_REG) == DT_REG)//如果读到的文件是普通文件则复制
                deal_copy(buf_oldfile, buf_newfile);    
            else if((ret_redir->d_type & DT_DIR) == DT_DIR)//如果读到的是目录则递归处理目录
                deal_dir(buf_oldfile, buf_newfile);
        }
    }
}

void get_allpath(char * buf_old, char * buf_new, const char * old_path, const char * new_path)
{
    char buf[PATH_SIZE] = {};/*存放当前路径*/
    getcwd(buf,PATH_SIZE);/*获取当前路径*/
    chdir(old_path);/*改变路径到source*/
    getcwd(buf_old,PATH_SIZE);/*获取source的绝对路径*/
    strcat(buf_old, "/");/*加上“/”以方便之后向buf_oldfile中补上具体文件名*/

    chdir(buf);/*回到原来目录*/
    /*与获取source绝对路径相同，获取target绝对路径，当然两次获取可以简化成一个函数，由于代码不是太多就没过多简化*/
    mkdir_newdir(new_path);//先创建一个空目录
    chdir(new_path);
    getcwd(buf_new, PATH_SIZE);
    strcat(buf_new,"/");

}
void init_allpath(char * buf_old, char * buf_new, char * buf_oldfile, char * buf_newfile, const char * name)
{
    /*清空source与target的带文件名的绝对路径缓冲区*/
    memset(buf_oldfile, 0, PATH_SIZE);
    memset(buf_newfile, 0, PATH_SIZE);
    /*将source的缓冲区更新为最新读到的source文件*/
    strcpy(buf_oldfile, buf_old);
    strcat(buf_oldfile, name);
    /*同上理*/
    strcpy(buf_newfile, buf_new);
    strcat(buf_newfile, name);

}
void mkdir_newdir(const char * new_path)
{
    /*目录不存在返回NULL，则创建空目录*/
    DIR * ret_opdir = opendir(new_path);
    if(ret_opdir == NULL){
        int ret_mkdir = mkdir(new_path, DIR_MODE);/*创建子目录*/
        if(ret_mkdir == -1)
            sys_err("mkdir newdir");
    }
}
void deal_copy(const char *old_file,const  char * pathname)
{
    struct stat get_message;
    int ret_stat = stat(pathname, &get_message);    
    if(ret_stat == -1 && errno != ENOENT)//文件信息读取失败，并且不是因为无该文件造成的
        sys_err("stat in copy.c of deal_copy");

    fd[0] = open(old_file, O_RDONLY);/*打开source文件*/
    if(fd[0] == -1)
        sys_err("open oldfile in copy_file");
    fd[1] = open(pathname, O_CREAT | O_TRUNC | O_RDWR, FILE_MODE);/*创建并打开target文件*/
    if(fd[1] == -1)
        sys_err("open newfile in copy_file");

    read_write();/*进行source的读与对应的target的写(复制)*/

    close(fd[0]);
    close(fd[1]);
}   
void read_write(void)
{
    char buf[BUF_SIZE] = {};
    int ret_read, ret_write;
    while((ret_read = read(fd[0], buf, BUF_SIZE)) > 0){/*读到EOF时返回0结束*/
        ret_write = write(fd[1], buf, strlen(buf));
        /*等效于ret_write = write(fd[1], buf, ret_read);*/
        if(ret_write == -1)
            sys_err("write newfile");
    }
}


int main(int argc, char ** argv){
    if(argc != 3){
        printf("too few or lost parameter!\n");
        exit(EXIT_FAILURE);
    }
    else if(1){
        struct stat get_message;
        int ret_stat = stat(argv[1],&get_message);
        if(ret_stat == -1)
            sys_err("stat argv[1]");

        if(S_ISDIR(get_message.st_mode))
            deal_dir(argv[1],argv[2]);//如果argcv[1]是目录则按目录递归处理
        else{//否则按照文件直接复制,局限性：复制单个文件时只能向当前目录复制
            if(strcmp(argv[1],argv[2]) == 0){
                printf("The same filename of parameter is error!\n");
                exit(EXIT_FAILURE);
            }
            deal_copy(argv[1],argv[2]);
        }
    }

    return 0;
}
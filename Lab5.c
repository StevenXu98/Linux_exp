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
char* destinationPath;
char* destinationPath2;
int is_dir(char* path)
{
	struct stat st;
	stat(path,&st);
	if(S_ISDIR(st.st_mode))
		return 1;
	else 
		return 0;
}

void copy_file(char* source_path,char* destination_path)
{
	char buffer[1024];
	FILE *in,*out;
	int len;
	if((in=fopen(source_path,"r"))==NULL)
	{
		printf("Can't open the sourcefile");
	}
	if((out=fopen(destination_path,"w"))==NULL)
	{
		printf("Can't open the destinationfile");
	}
	while((len=fread(buffer,1,1024,in))>0)
	{
		fwrite(buffer,1,len,out);
	}
	fclose(out);
	fclose(in);
}

void copy_folder(char* source_path,char* destination_path)
{
	DIR* des_dir=opendir(destination_path);
	if(!des_dir)
	{
		if(mkdir(destination_path,0777))
		{
			printf("Fail to create a folder.");
		}
	}
	struct dirent* file_con;
	DIR* dp=opendir(source_path);
	while((file_con=readdir(dp))!=NULL)
	{
		char file_source_path[1024]={0};
		snprintf(file_source_path,sizeof(file_source_path),"%s/%s",source_path,file_con->d_name);

		char file_destination_path[1024]={0};
		snprintf(file_destination_path,sizeof(file_destination_path),"%s/%s",destination_path,file_con->d_name);
	
		if(is_dir(file_source_path))
		{
			if((strcmp(file_con->d_name,".")!=0)&&(strcmp(file_con->d_name,"..")!=0))
			{
				copy_folder(file_source_path,file_destination_path);
			}
		}
		else
		{
			copy_file(file_source_path,file_destination_path);
			printf("%s %s\n",file_source_path,file_destination_path);
		}
	}
    if (!des_dir)
    {
        closedir(des_dir);
    }
    if (!dp) 
    {
        closedir(dp); 
    }  
}


int mkdir(const char *path,mode_t mode);
void *childthread1(char* rep){
	char *buffer=rep;
	copy_folder(buffer,destinationPath);
	return (void *)0;
}

void *childthread2(char* rep){
	char *buffer=rep;
	copy_file(buffer,destinationPath2);
	printf("%s %s\n",buffer,destinationPath2);
	return (void *)0;
}


int pthread_create(pthread_t *restrict tidp,
				const pthread_attr_t *restrict attr,
				void *(*start_rtn)(void *), 
				void *restrict arg);

int pthread_join(pthread_t thread,void **rval_ptr);

int copy_process1(char* file_source_path,char* file_destination_path)
{
	int ret=0;
	pthread_t  tid;
	char* rep=file_source_path;
	printf("create childthread\n"); 
    ret=pthread_create(&tid,NULL,(void *) childthread1,(void*)rep);
	if(ret)
	{
		printf("ERROR in create childthread");
		return 1;
	}
	//execl("/home/tangjiaqi/test/copy", buffer,"/home/tangjiaqi/唐嘉岐",(char*)0);  
	pthread_join(tid,NULL);
	printf("childthread exit\n");
	return 0;
}

int copy_process2(char* file_source_path,char* file_destination_path)
{
	int ret=0;
	pthread_t  tid;
	char* rep=file_source_path;
	printf("create childthread\n"); 
    ret=pthread_create(&tid,NULL,(void *) childthread2,(void*)rep);
	if(ret)
	{
		printf("ERROR in create childthread");
		return 1;
	}
	//execl("/home/tangjiaqi/test/copy", buffer,"/home/tangjiaqi/唐嘉岐",(char*)0);  
	pthread_join(tid,NULL);
	printf("childthread exit\n");
	return 0;
}

int main(int argc, char *argv[])
{
	DIR* dp1;	
	char buffer[100];
	struct dirent* currentdp;
	struct stat currentstat;
	
	mkdir("/home/tangjiaqi/唐嘉岐",0777);
	

	char* getcwd(char *buf,size_t size);	
	if(getcwd(buffer,100))
	{
		printf("%s\n",buffer);
	}
	char* source_path=buffer;
	char* destination_path="/home/tangjiaqi/唐嘉岐";
	if((dp1=opendir(buffer))==NULL)
	{
		printf("Open directory fail\n" );
		exit(1);
	}
	while((currentdp=readdir(dp1)) !=NULL)
	{
		char file_source_path[1024]={0};
		snprintf(file_source_path,sizeof(file_source_path),"%s/%s",source_path,currentdp->d_name);

		char file_destination_path[1024]={0};
		snprintf(file_destination_path,sizeof(file_destination_path),"%s/%s",destination_path,currentdp->d_name);

		if(currentdp->d_name[0]!='.')
		{
			if(is_dir(file_source_path))
			{
				if((strcmp(currentdp->d_name,".")!=0)&&(strcmp(currentdp->d_name,"..")!=0))
				{
					destinationPath=file_destination_path;
					copy_process1(file_source_path,file_destination_path);
				}
			}
			else
			{
				destinationPath2=file_destination_path;
				copy_process2(file_source_path,file_destination_path);
			}
		}	
	}
	closedir(dp1);
	return 0;
}


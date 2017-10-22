// #include <apue.h>
// #include "dirent.h"
#include <pwd.h>
#include <grp.h>
// #include "time.h"
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

void print_error(char *errmsg)
{
	printf("ERROR: %s\n",errmsg);
	exit(0);
}
char *getformattime(time_t t)
{
	static char formattime[20];
	
	struct tm *lt;
	lt = localtime(&t);  
	char tmptime[24];  
	memset(tmptime, 0, sizeof(tmptime));  
	strftime(tmptime, 24, "%Y-%m-%d %H:%M:%S", lt);  

	strcpy(formattime,tmptime);
	return formattime;
}
char *getuserinfo(uid_t my_uid)
{
	char username[100];
	char groupname[100];

	static char userinfo[200];

	struct passwd *my_info;
	struct group *grp;

	my_info = getpwuid(my_uid);
	strcpy(username,my_info->pw_name);
	
	grp = getgrgid(my_info->pw_gid);
	if (!grp){
		strcpy(groupname,"-");
	}else{
		strcpy(groupname,grp->gr_name);
	}
	strcat(username,"\t");
	strcat(username,groupname);
	strcpy(userinfo,username);

	return userinfo;
}

char *getinfo(char *path)
{
	static char str[1000];
	
	struct stat info;
	stat(path,&info);
	strcpy(str, "----------"); 

	if (S_ISDIR(info.st_mode)){
		str[0] = 'd';
	}else if (S_ISCHR(info.st_mode)) {
		str[0] = 'c';
	}else if (S_ISBLK(info.st_mode)) {
		str[0] = 'b';
	} else{
		str[0] = '-';
	}

	if (info.st_mode & S_IRUSR) {
		str[1] = 'r';  
	}
	 
	if (info.st_mode & S_IWUSR) {
		str[2] = 'w';
	}

	if (info.st_mode & S_IXUSR) {
		str[3] = 'x';
	}

	if (info.st_mode & S_IRGRP) {
		str[4] = 'r';
	}

	if (info.st_mode & S_IWGRP) {
		str[5] = 'w';
	}

	if (info.st_mode & S_IXGRP){
		str[6] = 'x';
	}

	if (info.st_mode & S_IROTH) {
		str[7] = 'r';
	} 

	if (info.st_mode & S_IWOTH) {
		str[8] = 'w';
	}

	if (info.st_mode & S_IXOTH) {
		str[9] = 'x';
	}
	
	char tmp[20];

	strcat(str," ");
	sprintf(tmp,"%ld",info.st_nlink);
	strcat(str,tmp);

	strcat(str," ");
	strcat(str,getuserinfo(info.st_uid));

	strcat(str," ");
	sprintf(tmp,"%20ld",info.st_size);
	strcat(str,tmp);

	strcat(str," ");
	strcat(str,getformattime(info.st_mtime));
	return str;
		
}

int main(int argc,char *argv[])
{
	DIR *dp; 
	struct dirent *dirp;
	int max_dir_length = sysconf(_PC_PATH_MAX); //△sysconf
	char dir[max_dir_length];

	if (argc != 2) {
		char buf[max_dir_length];
		if(getcwd(buf,max_dir_length)){
			strcpy(dir,buf);
		}else{
			print_error("Dir is too long or something error!");
		}
	}else{
		strcpy(dir,argv[1]);
	}
   
	if ((dp = opendir(dir)) == NULL) {
		print_error("Read dir failed!");
	}

	char tmp_dir[max_dir_length];
	while ((dirp = readdir(dp)) != NULL) {
		if(strcmp(dirp->d_name,"..") && strcmp(dirp->d_name,".")){
			strcpy(tmp_dir,dir);
			strcat(tmp_dir,"/");
			printf("%s %s\n",getinfo(strcat(tmp_dir,dirp->d_name)),dirp->d_name);
		}
	}
   
	closedir(dp);
	exit(0);
  
}

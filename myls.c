#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <dirent.h>

void display_file(char *pathname)
{
	struct stat f_info;
	struct tm *ptm;

	printf("pathname : %s.\n",pathname);

	if(stat(pathname,&f_info) < 0)
	{
		fprintf(stderr,"Fail to stat %s : %s.\n",pathname,strerror(errno));
		return;
	}

	//文件类型
	switch(f_info.st_mode & S_IFMT)
	{
	case S_IFSOCK:
		putchar('s');
		break;

	case S_IFLNK:
		putchar('l');
		break;

	case S_IFREG:
		putchar('-');
		break;

	case S_IFIFO:
		putchar('p');
		break;

	case S_IFCHR:
		putchar('c');
		break;

	case S_IFBLK:
		putchar('b');
		break;

	case S_IFDIR:
		putchar('d');
		break;
	}
	
	//权限
	if(f_info.st_mode & S_IRUSR)
		putchar('r');
	else
		putchar('-');

	if(f_info.st_mode & S_IWUSR)
		putchar('w');
	else
		putchar('-');

	if(f_info.st_mode & S_IXUSR)
		putchar('x');
	else
		putchar('-');

	if(f_info.st_mode & S_IRGRP)
		putchar('r');
	else
		putchar('-');

	if(f_info.st_mode & S_IWGRP)
		putchar('w');
	else
		putchar('-');

	if(f_info.st_mode & S_IXGRP)
		putchar('x');
	else
		putchar('-');
	
	if(f_info.st_mode & S_IROTH)
		putchar('r');
	else
		putchar('-');
	
	if(f_info.st_mode & S_IWOTH)
		putchar('w');
	else
		putchar('-');

	if(f_info.st_mode & S_IXOTH)
		putchar('x');
	else
		putchar('-');

	printf(" %d",f_info.st_nlink);
	
	//文件所有者 
	printf("   %s",getpwuid(f_info.st_uid)->pw_name);
	
	//所在组 
	printf("   %s",getgrgid(f_info.st_gid)->gr_name);

	//文件大小 	
	printf("   %ld",f_info.st_size);

	//最后一次修改的时间
	ptm = localtime(&f_info.st_mtime);
	fprintf(stdout,"   %d-%d-%d  %d:%d:%d  ",ptm->tm_year + 1900,ptm->tm_mon + 1,\
			ptm->tm_mday,ptm->tm_hour,ptm->tm_min,ptm->tm_sec);
	
	return;
}

void display_dir(const char *dirname)
{
	DIR *pdir;
	struct dirent *pdirent;
	int count1 = 0,count2 = 0;
	char pathname[1024];

	printf("dirname : %s.\n",dirname);

	if((pdir = opendir(dirname)) == NULL)
	{
		fprintf(stderr,"Fail to opendir %s : %s.\n",dirname,strerror(errno));
		return;
	}

	while( (pdirent = readdir(pdir)) != NULL )
	{
		//.  ..
		/*
		if(strcmp(pdirent->d_name,".") == 0 || strcmp(pdirent->d_name,"..") == 0)
			continue;
 		*/
		/*
		if(strncmp(pdirent->d_name,".",1) == 0)
			continue;
		*/
		if(pdirent->d_name[0] == '.')
			continue;
		
		//pathname:dirname/filename
		//dirname:/home/mrxiaobin 
		//       :/home/mrxiaobin/
		if(dirname[strlen(dirname) - 1] == '/')
		{
			sprintf(pathname,"%s%s",dirname,pdirent->d_name);
		
		}else{
			sprintf(pathname,"%s/%s",dirname,pdirent->d_name);
		}

		display_file(pathname);

		printf("  %s\n",pdirent->d_name);

		switch(pdirent->d_type)
		{
		case DT_DIR:
			count1 ++;
			break;

		case DT_REG:
			count2 ++;
			break;
		}
	}

	printf("File : %d  Dir : %d.\n",count2,count1);

	return;
}


//./a.out  dirname
int main(int argc, const char *argv[])
{
	if(argc < 2)
	{
		fprintf(stderr,"Usage : %s argv[1].\n",argv[0]);
		return -1;
	}

	display_dir(argv[1]);
	
	return 0;
}

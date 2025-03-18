// server FIFO 
 
#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<errno.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdlib.h>
 
#define ERR_EXIT(m)\
do{\
	perror(m);\
	exit(EXIT_FAILURE);\
}while(0);
 
 
int main()
{
	umask(0);
	
	int rc = mkfifo("mypipe",0644);

	if(rc < 0)
	{
	    if (errno != EEXIST)		
		ERR_EXIT("mkfifo");
	}

	int rfd = open("mypipe",O_RDONLY|O_CREAT);

	if(rfd < 0)
	{
		ERR_EXIT("open");
	}
 
	char buf[1024];

	while(1)
	{
		buf[0] = 0;
		printf("please wait...\n");
		ssize_t s = read(rfd,buf,sizeof(buf)-1);
		if(s > 0)
		{
			buf[s-1] = 0;
			printf("client say# %s\n",buf);
		}
		else if(s == 0)
		{
			printf("client quit,exit now!\n");
			exit(EXIT_SUCCESS);
		}
		else
		{
			ERR_EXIT("read");
		}
	}
 
	close(rfd);
	return 0;
}


#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include<errno.h>
#include<string.h>
#include<sys/ioctl.h>
#include "pmdev.h"
#include "nvm.h"
#define MAXSIZE 100


int main(int argc,char **argv)
{

	int ret,cmd,fd,fd1,quantum,qset,temp;
	int volume_no; 
	fd=open("/dev/PMDev",O_RDWR);
	if(fd<0){
		printf("打开设备失败");
	}else{
		
		cmd=DELETEVOLUME;
		volume_no=atoi(argv[1]);
		
		temp=ioctl(fd,cmd,&volume_no);
		if(temp<0){
			printf("删除卷失败！\n");	
		}else{
			printf("删除卷成功！\n");	
		}	
	}
    close(fd);    
    return 0;
}


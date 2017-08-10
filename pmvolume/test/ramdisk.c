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

int main(int argc,char **argv)
{
	int ret,cmd,fd,fd1,quantum,qset,temp,ls1,ls2;
	int i;
	fd=open("/dev/PMDev",O_RDWR);
	if(fd<0){
    		printf("打开设备失败\n");
    	}else{
		printf("打开设备成功\n");
		cmd=RAMDISK;
//cmd=VOLUME_SIZE_SET;
		temp=ioctl(fd,cmd,NULL);
		if(temp<0){
			printf("失败！\n");	
		}else{
			printf("SUCCESS!\n");
		}
	}
	close(fd);  
	return 0;
}

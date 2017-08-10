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
#define SEEK_BACK 3

int main(int argc,char **argv)
{

	int ret,cmd,fd,fd1,quantum,qset,temp,ls1,ls2;
	int i;
	char buff[4220];
	fd=open(argv[1],O_RDWR);
	if(fd<0){
    		printf("打开设备失败\n");
    	}else{
		printf("打开设备成功\n");
		read(fd,buff,sizeof(buff));
		printf("读取到的值: %s\n",buff);
		for(i=0;i<4220;i++){
			if(buff[i]!='\0'){
				printf("READ的数据:%d %c\n",i,buff[i]);
			}	
		}
		
	}	

}

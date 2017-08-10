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
/**
*SEEK_SET:从起始位置开始
*SEEK_CUR:从当前位置开始
*SEEK_END:从结尾位置开始
*/
int main(int argc,char **argv)
{
	int ret,cmd,fd,fd1,quantum,qset,temp,ls1,ls2;
	fd=open(argv[1],O_RDWR);
	if(fd<0){
    		printf("打开设备失败\n");
    	}else{
		printf("打开设备成功\n");
		int i;
		char set[4220]="";
		set[0]='x';
		set[1]='x';
		set[2]='3';
		set[3]='w';
		set[4]='5';
		set[5]='q';
		set[100]='d';
		set[1000]='e';
		set[4094]='6';
		set[4095]='g';
		set[4096]='z';
		set[4097]='k';
		set[4100]='8';
		set[4219]='4';

		write(fd,set,sizeof(set));
		close(fd);  
		return 0;
	}
}

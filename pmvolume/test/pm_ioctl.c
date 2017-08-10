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
	int ret,cmd,fd,fd1,quantum,qset,temp,i;
	///dev/PMDevice1
	fd=open(argv[1],O_RDWR);
	if(fd<0){
    		printf("打开设备失败\n");
    	}else{
		printf("打开设备成功\n");
		
		cmd=VOLUME_SIZE_GET;
		struct ioc_val val;
		val.volume_no=1;
		temp=ioctl(fd,cmd,&val);
		if(temp<0){
			printf("失败！\n");
		}else{		
		
		struct device_val *data=(struct device_val*)malloc(sizeof(struct device_val)*(val.value));
		cmd=GET_RANGESET;
		temp=ioctl(fd,cmd,data);
		if(temp<0){
			printf("失败！\n");	
		}else{
			printf("成功！\n");
			printf("**%d\n**",val.value);
			for(i=0;i<(val.value);i++){
				
				printf("device_no:%lu\n",(data+i)->device_no);
				printf("start:%lu\n",(data+i)->start);
				printf("size:%lu(pgs) %lu(B)\n",(data+i)->size,((data+i)->size)*4096);
				printf("**********************\n");
			}	
		}
	}	
	
	}
	close(fd);    
	return 0;
}

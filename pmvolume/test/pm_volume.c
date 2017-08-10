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
#define MAXSIZE 4000


int main(int argc,char **argv)
{
	int ret,cmd,fd,fd1,quantum,qset,temp;
	struct ioc_val data;
	struct device_val address;
	struct vol_val vv;
	int v_n;  //设备号
	int volu_no;
	struct node *head = (struct node*)malloc(sizeof(struct node));	
	initList(head);
	long start=0x200000000>>12;  //pm_volume起始地址(???)8G
	int device_no[MAXSIZE];

	/****/
	int device_start[MAXSIZE];
	/****/

	int device_size[MAXSIZE];
	int device_num;
	int volume_no; 


	fd=open("/dev/PMDev",O_RDWR);
	if(fd<0){
		printf("打开设备失败\n");
	}else{
	printf("打开设备成功\n");

	FILE * fp;
	int i=0;	
	char buffer[80];
	char delims[] = " ";
	int tmp[MAXSIZE];
	for(i=0; i<MAXSIZE; i++){
		tmp[i] = 0;
	}
	i = 0;

	//"cat /home/linux/code_chen/init1"

	char str[100]={'\0'};
	strcat(str,argv[1]);
	strcat(str," ");
	strcat(str,argv[2]);
	fp=popen(str, "r");	
//	fgets(buffer,sizeof(buffer),fp);

	while(!feof(fp)){

		fgets(buffer,sizeof(buffer),fp);

		if(buffer[0] == '#'){		
			//printf("%s",buffer);
		}else{
			char *result = NULL;
			result = strtok(buffer, delims);
			while(result != NULL) {
				//!?
				tmp[i] = atoi(result);	
				insertLastList(head,atoi(result));		
				result = strtok(NULL, delims);
			}

		}

	}	
	pclose(fp);

/*
struct  node *nhead=head;
while(nhead!=NULL){
	printf("%d\n",nhead->num);
	nhead=nhead->next;
}
*/

	int _num=head->next->num;
	int j,k;	
	Del_Node(head,1);

	for(j=0; j<_num; j++) {

		volume_no= head->next->num;
		Del_Node(head,1);
		cmd = CREATEDEVICE;

		temp=ioctl(fd,cmd,&volume_no);	
		if(temp<0){
		
			printf("创建1个设备失败！\n");	
		}else{
			printf("创建1个设备成功！\n");	
		}

		int device_num = head->next->num;
		Del_Node(head,1);		
		for(k=0; k< device_num; k++){
			device_no[k]=head->next->num;
			Del_Node(head,1);
			device_start[k]=head->next->num;
			Del_Node(head,1);

			device_size[k]=head->next->num;
			Del_Node(head,1);
		}	
		int m;
		int kk;
		
/*
		for(kk=0; kk< device_num; kk++){
			printf("device_no:%d\n",device_no[kk]);
			printf("device_start:%d\n",device_start[kk]);
			printf("device_size:%d\n",device_size[kk]);
		}
*/
		
		vv.device_no = device_no;
		vv.device_start = device_start;
		vv.device_size = device_size;
		vv.device_num = device_num;
		vv.volume_no = volume_no;
		vv.start = start + volume_no*start ;  //每个volume的起始地址

		cmd=CREATEVOLUME;
		temp=ioctl(fd,cmd,&vv);
		if(temp<0){
			printf("创建卷%d失败！\n",volume_no);	
		}else{
			printf("创建卷%d成功！\n",volume_no);	
		}
	}

    }
    close(fd);    
    return 0;
}


/*************************************************************************
	> File Name: memset_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年07月06日 星期四 21时33分04秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main() {
    int *addr = (int *)malloc(sizeof(int) * 10);
    int i = 0;
    for(;i < 10;i++) {
        addr[i] = i;
        printf("data %d's value is %d.\n", i+1, addr[i]);
    }

    memset(addr, 0, sizeof(int)*10);
    for(i = 0;i < 10;i++) {
        printf("data %d's value is %d.\n", i+1, addr[i]);
    }
    return 0;
}

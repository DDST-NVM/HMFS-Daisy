/*************************************************************************
	> File Name: test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年03月03日 星期五 17时12分03秒
 ************************************************************************/

#include<stdio.h>
int max(int, int);
int main(){
    int a = 5;
    int b = 10;
    printf("a = %d\n",a);
    int max_num = max(a, b);
    printf("max_num = %d\n", max_num);
    return 0;
}

int max(int a, int b) {
    if (a > b) 
    return a;
    else 
    return b;
}

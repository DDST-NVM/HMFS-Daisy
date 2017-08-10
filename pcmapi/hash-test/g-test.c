/*************************************************************************
	> File Name: g-test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年08月07日 星期一 22时34分29秒
 ************************************************************************/

#include<stdio.h>
#include "p_mmap.h" 

typedef struct student {
    int id;
    char *name;
    int age;
} stu;

int main() {
    printf("---------------------Check p_init Start-----------------------\n");
    // int iRet = p_init(64 * 1024);
    int iRet = p_init(200 * 1024 * 1024); // pre-allocate 200MB memory
    if (iRet < 0) {
        printf("Error: p_init failed!\n");
        return -1;
    }
    void *base = p_get_base();
    printf("pBaseAddr is %p.\n", base);
    printf("---------------------Check p_init Over-----------------------\n");
    iRet = p_clear(); // memset call and check
    printf("---------------------Check p_malloc Start-----------------------\n");
    stu *stu1 = (stu *)p_malloc(100, sizeof(stu));
    printf("The allocated address is %p.\n", stu1);
    void *startAddr = (void *)stu1;
    int nid = *((int *)(startAddr - 3 * sizeof(int)));
    int length = *((int *)(startAddr - 2 * sizeof(int)));
    int nextLoc = *((int *)(startAddr - 1 * sizeof(int)));
    printf("The metadata for struct should be nid: %d, length: %d and nextLoc: %d.\n", nid, length, nextLoc);
    stu1->id = 10086;
    stu1->name = "Huang";
    stu1->age = 21;

    printf("Input message: id: %d, name: %s, age: %d.\n", stu1->id, stu1->name, stu1->age);

    int *testNum = (int *)p_malloc(101, sizeof(int));
    nid = *(testNum-3);
    length = *(testNum-2);
    nextLoc = *(testNum-1);
    printf("The metatdata for integer should be nid: %d, length: %d and nextLoc: %d.\n", nid, length, nextLoc);
    *testNum = 10;
    printf("Input message: *testNum=%d.\n", *testNum);

    printf("---------------------Check p_malloc Over-----------------------\n");
    
    printf("---------------------Check p_get_malloc Start-----------------------\n");
    stu *stuAddr = (stu *)p_get_malloc(100);
    int *numAddr = (int *)p_get_malloc(101);
    printf("p_get_malloc obtains stuAddr (%p) and numAddr (%p).\n", stuAddr, numAddr);
    printf("Elements for stuAddr: id: %d, name: %s, age: %d.\n", stuAddr->id, stuAddr->name, stuAddr->age);
    printf("Element for numAddr: %d.\n", *numAddr);

    printf("---------------------Check p_get_malloc Over-----------------------\n");

    printf("---------------------Check p_free Start-----------------------\n");
    iRet = p_free(100);
    iRet = p_free(101);
    
    void *nullAddr = p_get_malloc(101);
    if (nullAddr == NULL) {
        printf("Free successes!\n");
    }
    
    printf("---------------------Check p_free Over-----------------------\n");
}

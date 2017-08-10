#include "test.h"
#include "p_mmap.h"
#include "stdlib.h"

/**
* 这段程序的作用是在PCM中新建一个链表，绑定一个用户指定的ID，并支持在下一次启动时，根据ID将该链表重新构建出来。
* How to run for test:
* ./ptest p (init a large memory chunk for further memory allocation)
* ./ptest gba (get base address of allocated heap memory)
* ./ptest m 10 (malloc 10 integer memory)
* ./ptest gm (get malloc data of specified id)
* ./ptest f (free any region of specified id)
* ./ptest n (alloc and write a large chunk of memory on scm)
* ./ptest d (delete the large memory chunk allocated by p_new)
* ./ptest gn (re-map and read a large chunk of memory with p_new)
* ./ptest c (reset the memory allocator and set all bits to 0)
* ./ptest lc (create a log)
* ./ptest ld (delete a log)
* ./ptest ts (start a transaction with id)
* ./ptest te (end a transaction with id)
* ./ptest tr (record kv pair during a transaction)
* ./ptest comb1 (test p_init+p_malloc)
* ./ptest comb2 (test p_new + p_get)
* ./ptest comb3 (test p_malloc + p_get_malloc)
* ./ptest comb4 (test log_create + transaction_start)
* ./ptest comb5 (test transaction_end + log_delete)
*/

typedef struct stu{
	char *name;
	int age;
}stu;

int main(int argc, char **argv) {
    int iRet = 0;
    char *ptr = NULL;
    int size = 200 * 1024 * 1024;
    if (argc == 2 && argv[1][0] == 'p') { // p_init test
        printf("\n1. p_init test. If successful, return the metadata for inition region, else return failed info.\n");
        
    }
    iRet = p_init(size);
    if (iRet < 0) {
        printf("error: p_init\n");
        return -1;
    }

    if (argc == 2 && argv[1][0] == 'p') { // p_init test
        p_clear(); // memset all memory exceot the metadata
        int *metaAddr = (int *)p_get_base();
        printf("-----------------------------------------------------------\n");
        printf("baseAddr: %-10p\n", metaAddr);
        printf("startLoc: %-10d\n", *(metaAddr - 1));
        printf("freeFlag: %-10d\n", *(metaAddr));
        printf("freeSize: %-10d\n", *(metaAddr + 1));
        printf("nextLoc : %-10d\n", *(metaAddr + 2));
        printf("-----------------------------------------------------------\n\n");
    }

    else if (argc == 2 && argv[1][0] == 'g' && argv[1][1] == 'b' && argv[1][2] == 'a') {
        printf("\n2. p_get_base test. If successful, return a non-zero base address value, else return NULL address.\n");
        printf("-----------------------------------------------------------\n");
    	char *base = (char*)p_get_base();
        printf("Base address: %p.\n", base);
        printf("-----------------------------------------------------------\n\n");
    }

    else if (argc == 2 && argv[1][0] == 'c') { /* reset the memory allocator */ 
        printf("\n6.p_clear test. If successful, then all the memory initiated will be set to 0 in bits;else return failed info.\n");
        printf("-----------------------------------------------------------\n");
        iRet = p_clear();
        if (iRet < 0) {
            printf("error: p_clear\n");
            return -1;
        }
        int *metaAddr = (int *)p_get_base();
        printf("Metadata has changed after p_clear():\n");
        printf("baseAddr: %-10p\n", metaAddr);
        printf("startLoc: %-10d\n", *(metaAddr - 1));
        printf("freeFlag: %-10d\n", *(metaAddr));
        printf("freeSize: %-10d\n", *(metaAddr + 1));
        printf("nextLoc : %-10d\n", *(metaAddr + 2));
        printf("-----------------------------------------------------------\n\n");
    } 
    
    else if (argc == 2 && argv[1][0] == 'm') { /* malloc memory for stu structure and integer*/
    	int stu_id = 100;
        int int_id = 200;
    	int t;
    	int *metaAddr = (int *)p_get_base();
        printf("\n3. malloc test. If successful, return allocated memory address for each data element, else return failed info.\n");
        stu *stu_obj = (stu *)p_malloc(stu_id, sizeof(stu));
        int *int_obj = (int *)p_malloc(int_id, sizeof(int));
        stu_obj->name = "Kaixin";
        stu_obj->age = 21;
        *int_obj = 888;
        printf("-----------------------------------------------------------\n");
        printf("stu_obj address: %-10p; stu_obj id: %-10d\n", stu_obj, stu_id);
        printf("int_obj address: %-10p; int_obj id: %-10d\n", int_obj, int_id);
        printf("stu_obj        : | name  = %-10s | age = %-10d\n", stu_obj->name, stu_obj->age);
        printf("int_obj        : | value = %-10d\n", *int_obj);
        printf("\nAfter memory allocation, metadata should change as follows:\n");
        printf("baseAddr       : %-10p\n", metaAddr);
        printf("startLoc       : %-10d\n", *(metaAddr - 1));
        printf("freeFlag       : %-10d\n", *(metaAddr));
        printf("freeSize       : %-10d\n", *(metaAddr + 1));
        printf("nextLoc        : %-10d\n", *(metaAddr + 2));
        printf("-----------------------------------------------------------\n\n");
    }   
    else if (argc == 2 && argv[1][0] == 'g' && argv[1][1] == 'm') {
        printf("\n4. p_get_malloc test. If successful, the stu_obj and int_obj can be retrieved with corresponding ids, else return failed info.\n");
        int stu_id = 100;
        int int_id = 200;
        stu *stu_get = (stu *)p_get_malloc(stu_id);
        int *int_get = (int *)p_get_malloc(int_id);
        printf("-----------------------------------------------------------\n");
        printf("stu_get address : %-10p; stu_get id: %-10d\n", stu_get, stu_id);
        printf("int_get address : %-10p; int_get id: %-10d\n", int_get, int_id);
        printf("stu_get metadata: | id    = %-10d | length = %-10d | nextLoc = %-10d\n", *((int *)stu_get-3), *((int *)stu_get-2), *((int *)stu_get-1));
        printf("int_get metadata: | id    = %-10d | length = %-10d | nextLoc = %-10d\n", *((int *)int_get-3), *((int *)int_get-2), *((int *)int_get-1));
        printf("stu_get         : | name  = %-10s | age    = %-10d\n", stu_get->name, stu_get->age);
        printf("int_get         : | value = %-10d\n", *int_get);
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 3 && argv[1][0] == 'f') {
        printf("\n5. p_free test. If successful, the stu_obj will be freed and metadata should change, else return failed info.\n");
        int free_id = atoi(argv[2]);
        int *free_obj = (int *)p_get_malloc(free_id);
        int retVal = p_free(free_id);
        printf("-----------------------------------------------------------\n");
        printf("free_id          : %-10d\n", free_id);
        printf("free_obj metadata: | id = %-10d | length = %-10d | nextLoc = %-10d\n", *(free_obj-3), *(free_obj-2), *(free_obj-1));
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 'n') {
        printf("\n7. p_new test. If successful, a large memory will be allocated for an array; else return failed info.\n");
        int new_id = 10000;
        int new_size = 8*1024;
        int *buf_obj = p_new(new_id, new_size);
        int i;
        for (i = 0;i < new_size/sizeof(int);i++) {
            buf_obj[i] = i;
        }
        printf("-----------------------------------------------------------\n");
        printf("buf_obj address: %-10p; buf_obj id: %-10d; buf_obj size: %-10d\n", buf_obj, new_id, new_size);
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 'g' && argv[1][1] == 'n') {
        printf("\n8. p_get test. If successful, the array value will be read out; else return failed info.\n");
        int new_id = 10000;
        int new_size = 8*1024;
        int *buf_get = p_get(new_id, new_size);
        int i;
        printf("-----------------------------------------------------------\n");
        printf("buf_get address: %-10p; buf_get id: %-10d\n", buf_get, new_id);
        for (i = 0;i < 100;i++) {
            if (buf_get[i] != i) {
                printf("Check error! data = %-10d, i = %-10d\n", buf_get[i], i);
            }
            else {
                printf("Check correct: buf_get[%3d] = buf_obj[%3d] = %3d\n", i, i, i);
            }
        }
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 'd') {
        printf("\n9. p_delete test. If successful, the large memory for the array will be recycled; else the array will still be accessed.\n");
        int retVal = p_delete(10000);
        printf("-----------------------------------------------------------\n");
        printf("Please re-execute p_get to certify the delete operation.\n");
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 'l' && argv[1][1] == 'c') {
        printf("\n10. log_create test. If successful, a log address and metadata will be returned; else return failed info.\n ");
        unsigned long log_id = 200;
        unsigned long *log_addr = log_create(log_id);
        printf("-----------------------------------------------------------\n");
        printf("log_addr: %-10p; log_id: %-10lu\n", log_addr, log_id);
        printf("metadata: START    = %-10lu\n", *log_addr);
        printf("metadata: PID      = %-10lu\n", *(log_addr+1));
        printf("metadata: END      = %-10lu\n", *(log_addr+2));
        printf("metadata: TS       = %-10lu\n", *(log_addr+3));
        printf("metadata: CNT_MLC  = %-10lu\n", *(log_addr+4));
        printf("metadata: CNT_FREE = %-10lu\n", *(log_addr+5));
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 't' && argv[1][1] == 's') {
        printf("\n11. transaction_start test. If successful, the log metadata should change; else return failed info.\n");
        unsigned long log_id = 200;
        unsigned long *log_addr = transaction_start(log_id);
        printf("-----------------------------------------------------------\n");
        printf("log_addr: %-10p; log_id: %-10lu\n", log_addr, log_id);
        printf("metadata: START    = %-10lu\n", *log_addr);
        printf("metadata: PID      = %-10lu\n", *(log_addr+1));
        printf("metadata: END      = %-10lu\n", *(log_addr+2));
        printf("metadata: TS       = %-10lu\n", *(log_addr+3));
        printf("metadata: CNT_MLC  = %-10lu\n", *(log_addr+4));
        printf("metadata: CNT_FREE = %-10lu\n", *(log_addr+5));
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 't' && argv[1][1] == 'r') {
        printf("\n12. transaction_record test. If successful, a log record should be persisted in log space; else return failed info.\n");
        unsigned long log_id = 200;
        unsigned long *int_addr = (unsigned long *)p_get_malloc(log_id);
        unsigned long *log_addr = transaction_record(log_id, int_addr);
        printf("-----------------------------------------------------------\n");
        printf("log_addr  : %-10p; log_id: %-10lu\n", log_addr, log_id);
        printf("metadata  : START    = %-10lu\n", *log_addr);
        printf("metadata  : PID      = %-10lu\n", *(log_addr+1));
        printf("metadata  : END      = %-10lu\n", *(log_addr+2));
        printf("metadata  : TS       = %-10lu\n", *(log_addr+3));
        printf("metadata  : CNT_MLC  = %-10lu\n", *(log_addr+4));
        printf("metadata  : CNT_FREE = %-10lu\n", *(log_addr+5));
        printf("log record: address  = %-10p; value = %-10lu\n", int_addr, *int_addr);
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 't' && argv[1][1] == 'e') {
        printf("\n13. transaction_end test. If successful, the log metadata should change; else return failed info.");
        unsigned long log_id = 200;
        unsigned long *log_addr = transaction_end(log_id);
        printf("-----------------------------------------------------------\n");
        printf("log_addr: %-10p; log_id: %-10lu\n", log_addr, log_id);
        printf("metadata: START    = %-10lu\n", *log_addr);
        printf("metadata: PID      = %-10lu\n", *(log_addr+1));
        printf("metadata: END      = %-10lu\n", *(log_addr+2));
        printf("metadata: TS       = %-10lu\n", *(log_addr+3));
        printf("metadata: CNT_MLC  = %-10lu\n", *(log_addr+4));
        printf("metadata: CNT_FREE = %-10lu\n", *(log_addr+5));
        printf("-----------------------------------------------------------\n\n");
    }
    else if (argc == 2 && argv[1][0] == 'l' && argv[1][1] == 'd') {
        printf("\n14. log_delete test. If successful, the log metadata should change; else return failed info.\n"); 
        unsigned long log_id = 200;
        unsigned long *log_addr = log_delete(log_id);
        printf("-----------------------------------------------------------\n");
        printf("log_addr: %-10p; log_id: %-10lu\n", log_addr, log_id);
        printf("metadata: START    = %-10lu\n", *log_addr);
        printf("metadata: PID      = %-10lu\n", *(log_addr+1));
        printf("metadata: END      = %-10lu\n", *(log_addr+2));
        printf("metadata: TS       = %-10lu\n", *(log_addr+3));
        printf("metadata: CNT_MLC  = %-10lu\n", *(log_addr+4));
        printf("metadata: CNT_FREE = %-10lu\n", *(log_addr+5));
        printf("-----------------------------------------------------------\n\n");
    }
    else {
        printf("\n-----------------------------------------------------------\n");
        printf("Test begins / ends!\n");
        printf("-----------------------------------------------------------\n\n");
    }
    return 0;
}

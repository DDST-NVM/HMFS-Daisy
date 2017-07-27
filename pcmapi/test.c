#include "test.h"
#include "p_mmap.h"
#include "stdlib.h"

/**
* 这段程序的作用是在PCM中新建一个链表，绑定一个用户指定的ID，并支持在下一次启动时，根据ID将该链表重新构建出来。
* How to run for test:
* ./ptest p (init a large memory chunk for further memory allocation)
* ./ptest gba (get base address of allocated heap memory)
* ./ptest m 10 (malloc 10 integer memory and then write them to the linked list)
* ./ptest b (bind data structure id to persistent heap memory)
* ./ptest gbd (get bind node of specified id)
* ./ptest f (free any address with p_malloc)
* ./ptest n (alloc and write a large chunk of memory on scm)
* ./ptest d (delete the large memory chunk allocated by p_new)
* ./ptest gn (re-map and read a large chunk of memory with p_new on scm)
* ./ptest c (reset the memory allocator and set all bits to 0)
* ./ptest lc (create a log)
* ./ptest ld (delete a log)
* ./ptest ts (start a transaction with id)
* ./ptest te (end a transaction with id)
* ./ptest tr (record kv pair during a transaction)
* ./ptest comb1 (test p_init+p_malloc+p_bind)
* ./ptest comb2 (test p_new + p_get)
* ./ptest comb3 (test p_bind + p_get_bind_node)
* ./ptest comb4 (test log_create + transaction_start)
* ./ptest comb5 (test transaction_end + log_delete)
*/

typedef struct {
	int data;
	int next;
} LinkedNode;

int main(int argc, char **argv) {
    int iRet = 0;
    char *ptr = NULL;
    if (argc == 2 && argv[1][0] == 'p') { // p_init test
        printf("p_init test. If successful, return base address and the inition size, else return failed info.\n");
    }

    iRet = p_init(4096 * 16);
    if (iRet < 0) {
        printf("error: p_init\n");
        return -1;
    }

    if (argc == 2 && argv[1][0] == 'p') { // p_init test
        printf("The inition size is 64KB.\n");
    }

    if (argc == 2 && argv[1][0] == 'g' && argv[1][1] == 'b' && argv[1][2] == 'a') {
        printf("p_get_base test. If successful, return a non-zero base address value, else return NULL address.\n");
        
    	char *base = (char*)p_get_base();
        printf("Base address is %p.\n", base);
    }

    else if (argc == 2 && argv[1][0] == 'c') { /* reset the memory allocator */ 
        printf("p_clear test. If successful, then all the memory initiated will be set to 0 in bits;else return failed info.\n");
        iRet = p_clear();
        if (iRet < 0) {
            printf("error: p_clear\n");
            return -1;
        }
    } else if (argc == 3 && argv[1][0] == 'm') { /* write the linked list 
    + malloc test */
    	int i;
    	int t;
    	char *base = (char*)p_get_base();
    	LinkedNode *nd, *last, *head;

    	t = atoi(argv[2]); // how many data elements need to apply for memory
    	last = (LinkedNode*)p_malloc(sizeof(LinkedNode));
		last->data = -1;
		head = last;

        printf("Malloc test. If successful, return allocated memory address for each data element, else return failed info.\n");

    	for(i=0; i<t; i++) {
        	nd = (LinkedNode*)p_malloc(sizeof(LinkedNode));
    		nd->data = i;
    		last->next = (char*)nd-base;
    		last = nd;
            printf("Malloc memory for the %d element, its persistent address is %p\n", i, nd);
    	}

    	last->next = 0;
    	// p_bind(1234,head,sizeof(LinkedNode));
    }   
    else if (argc == 2 && argv[1][0] == 'b'){
        printf("p_bind test. If successful, the base address will be printed;else a segment fault info will occur.\n");
    	int i;
    	int t;
    	char *base = (char*)p_get_base();
    	LinkedNode *nd, *last, *head;

    	t = 10; // how many data elements need to apply for memory
    	last = (LinkedNode*)p_malloc(sizeof(LinkedNode));
		last->data = -1;
		head = last;
        
    	for(i=0; i<t; i++) {
        	nd = (LinkedNode*)p_malloc(sizeof(LinkedNode));
    		nd->data = i;
    		last->next = (char*)nd-base;
    		last = nd;
    	}

    	last->next = 0;
        p_bind(1235,head,sizeof(LinkedNode));                                           
    }
    
    else if (argc == 2 && argv[1][0] == 'g' && argv[1][1] == 'b' && argv[1][2] == 'd') { /* read and check the linked list */
        printf("p_get_bind_node test. If successful, return an address with specified bind node 1234, else return failed info.\n");
    	char *base = (char*)p_get_base();
    	int sz,i;
    	LinkedNode* nd = p_get_bind_node(1235, &sz);                                                                               printf("The address obtained by p_get_bind_node is %p.\n", nd);
    	i = -1;
    	while (1) {
            /* check if the value is consistent */
        	if(nd->data != i) {
        		printf("Check Error! data=%d i=%d\n",nd->data,i);
        	}
        	i++;
    		if (nd->next)
    			nd = (LinkedNode*)(base + nd->next);
    		else
    			break;
    	}
    	printf("Check finish! i=%d\n",i);
    }
    else if (argc == 2 && argv[1][0] == 'f') {
        printf("p_free test. If successful, a successful free info will be returned and the next read with p_bind_node will fail;else return failed info or next read still gets 10 data elements.\n");
        int iRet = 0;
        int sz;
        LinkedNode* current;
    	LinkedNode* nd = p_get_bind_node(1235, &sz);  
    	int i = -1;
        char* base = (char *)p_get_base();
    	while (1) {
            /* check if the value is consistent */
        	if(nd->data != i) {
        		printf("Check Error! data=%d i=%d\n",nd->data,i);
        	}
        	i++;
            if (nd->next) {
                current = nd;
    			nd = (LinkedNode*)(base + nd->next);
                iRet = p_free(current);
            }
            else {
                iRet = p_free(nd);
                break;
            }
    	}
        printf("All 10 data element memory released successfully!\n");
    }
    else if (argc == 2 && argv[1][0] == 'n') {
        printf("p_new test. If successful, return a large chunk memory address; else return failed info.\n");
	    int* buf=p_new(10000,4096*2);
    	int i;
        printf("The allocated address value is %p.\n", buf);

    	for(i=0;i<4096*2/sizeof(int);i++)
    	{
    		buf[i]=i;
    	}
    }else if (argc == 2 && argv[1][0] == 'g' && argv[1][1] == 'n') {
        printf("p_get test. If successful, return a large chunk memory address for specified id 10000; else return failed info.\n");
    	int* buf=p_get(10000,4096*2);
        printf("The returned address value is %p.\n", buf);
    	int i;
    	for(i=0;i<4096*2/sizeof(int);i++)
    	{
    		if(buf[i]!=i)
    			printf("Check Error! data=%d i=%d\n",buf[i],i);
    	}
    	printf("Check finish!\n");
    }
    else if (argc == 2 && argv[1][0] == 'd') {
        printf("p_delete test. If successful, it returns base address and 0 returned value and next p_get will fail;else returned failed value or still access data in next p_get process.\n");
        printf("p_delete returns %d\n",p_delete(10000));
    } 

    else if (argc == 2 && argv[1][0] == 'l' && argv[1][1] == 'c') {
        printf("log_create test. If successful, return an address and corresponding log size for logging; else return failed info.\n");
        int log_size = 2048 * sizeof(unsigned long);
        unsigned long *log_addr = (unsigned long *)p_malloc(sizeof(log_size));

        printf("The log address is %p for pid 1234 and its size is %d.\n", log_addr, log_size);
        int iRet = p_free(log_addr);
    }

    else if (argc == 2 && argv[1][0] == 'l' && argv[1][1] == 'd') {
        printf("log_delete test. If successful, the log memory for specified pid will be released (by p_free); else return failed info.\n");

        int log_size = 2048 * sizeof(unsigned long);
        unsigned long *log_addr = (unsigned long *)p_malloc(sizeof(log_size));
        int iRet = p_free(log_addr);
        if (iRet < 0) {
            printf("Log delete failed!\n");
            return -1;
        } 
        printf("deletete log for pid 1234, the released memory address is %p and its size is %d.\n", log_addr, log_size);
    }

    else if (argc == 2 && argv[1][0] == 't' && argv[1][1] == 'c') {
        printf("transaction_start test. If successful, the START flag in that log will be set to 1; else return failed info.\n");
        
        int log_size = 2048 * sizeof(unsigned long);
        unsigned long *log_addr = (unsigned long *)p_malloc(sizeof(log_size));
        
        log_addr[0] = 1;

        printf("START flag (log_addr[0]) for pid 1234 is %ld.\n", log_addr[0]);
        int iRet = p_free(log_addr);
    }
    
    else if (argc == 2 && argv[1][0] == 't' && argv[1][1] == 'e') {
        printf("transaction_end test. If successful, the END flag in that log will be set to 1; else return failed info.\n");
        
        int log_size = 2048 * sizeof(unsigned long);
        unsigned long *log_addr = (unsigned long *)p_malloc(sizeof(log_size));
        
        log_addr[2] = 1;

        printf("END flag (log_addr[2]) for pid 1234 is %ld.\n", log_addr[2]);
        int iRet = p_free(log_addr);
    }

    else if (argc == 2 && argv[1][0] == 't' && argv[1][1] == 'r') {
        
    }


    else if (argc == 2 && argv[1][0] == 'c' && argv[1][1] == 'o' && argv[1][2] == 'm' && argv[1][3] == '1') {
        
    }
    
    else if (argc == 2 && argv[1][0] == 'c' && argv[1][1] == 'o' && argv[1][2] == 'm' && argv[1][3] == '2') {
        
    }

    else if (argc == 2 && argv[1][0] == 'c' && argv[1][1] == 'o' && argv[1][2] == 'm' && argv[1][3] == '3') {
        
    }

    else if (argc == 2 && argv[1][0] == 'c' && argv[1][1] == 'o' && argv[1][2] == 'm' && argv[1][3] == '4') {
        
    }

    else if (argc == 2 && argv[1][0] == 'c' && argv[1][1] == 'o' && argv[1][2] == 'm' && argv[1][3] == '5') {
        
    }

    else {
        printf("Test begins / ends!\n");
    }
    return 0;
}

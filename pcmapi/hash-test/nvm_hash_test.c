#include "p_mmap.h"
#include "stdlib.h"
#include "time.h"
/**
 * This program is to test the correctness of p_malloc()
 * We take LinkedList as an example
 * Firstly, we apply for a big (NVM) memory by calling p_init()
 * Then, each time we add a new node into the list, we use p_malloc(sizeof(LinkedNode))
 * At last, we bind the list data structure with a BIND_ID (such as 11) into big region with ID like 2345
 * To check if we can obtain the LinkedList data again, we first use p_get_base() to map the base_addr
 * Next we obtain the initial address of such a LinkedList by calling p_get_bind_node()
 * Then we output each node's data one after another, using (base + offset) to obtain the pointer address
 * We log the time for further test
*/

// Test using this link structure is okay ...
typedef struct {
	int data;
	int next;
} LinkedNode;

#define MAX_INIT_SIZE (200 * 1024 * 1024)
#define NUMMAX (1000 * 10000)
#define INIT_SIZE (MAX_INIT_SIZE * sizeof(int))
#define BIND_ID rand() % 1000
#define MALLOCSIZE (NUMMAX * sizeof(int))

#define INS_NUM 1000

int main() {
    int i;
    double duration;
    clock_t start, end;
    float throuput;

    int rand_loc = 1;
    int NUMINTS = 1000;
    int test_id = 100;

    srand((unsigned int)time(NULL));

    int iRet = 0;
    char *ptr = NULL;

    start = clock();
    iRet = p_init(MAX_INIT_SIZE);
    if (iRet < 0) {
        printf("Init error.\n");
        return -1;
    }
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("p_init time %f seconds\n", duration);

    start = clock();
    int *map = (int *)p_malloc(test_id, MALLOCSIZE);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("p_malloc time %f seconds\n", duration);

    for (;NUMINTS <= NUMMAX; NUMINTS = NUMINTS * 10) {
        start = clock();
        for (i = 0;i < NUMINTS;i++) {
            rand_loc = (int)((double)rand() / (RAND_MAX) * (NUMINTS-2) / 2);
            map[rand_loc] = i; // key
            map[rand_loc+1] = 2 * i; // value
            // A store instruction is in fact a persistence operation
        }
        end = clock();
        duration = (double)(end - start) / CLOCKS_PER_SEC;
        throuput = NUMINTS / duration;
        printf("-----------------------------------------------------------------------------\n");
        printf("data number: %10d | time delay: %10f(ms) | throuput: %10f(Ops/Sec)\n", NUMINTS, duration*1000, throuput);
        printf("-----------------------------------------------------------------------------\n\n");
    }
    start = clock();
    p_free(test_id);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Free time %f seconds\n", duration);
    return 0;
}

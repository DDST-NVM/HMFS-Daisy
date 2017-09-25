#include "p_mmap.h"
#include "stdlib.h"
#include "time.h"
#include <stdio.h>

/**
 * This program is written for real-time workload running with NVM usage monitoring for Redis SET operations
 * The benchmark flow is as follows:
 * 1. Set three random allocation size with ID: (~20M,100), (~20M,200), (~60M,300)
 * 2. Set a random flag (0 or 1) to decide if a write-to-file flush need to be done after each allocation
 * 3. free the allocated NVM region in the sequence of (1, 3, 2)
 * 4. Set a random flag (0 or 1) to decide if a write-to-file flush need to  be done after each allocation
*/

void file_record(FILE *fp) {
    int total = 200; // in MB format
    int free = get_free_size() / (1024 * 1024);
    int used = total - free; 
    // test output 
    fseek(fp, 0, SEEK_SET);
    printf("redisbench-debug: used NVM is %d MB.\n", used);
    fputs("Redis ", fp);
    // putw(used, fp);
    fprintf(fp, "%d", used);
    fputs("\n", fp);
}


int main() {
    int iRet = 0;
    int ID_1 = 100;
    int ID_2 = 200;
    int ID_3 = 300;
    int rand_size_1, rand_size_2, rand_size_3; // in MB format
    srand((unsigned int)time(NULL));

    // Specify the size of native heap
    int size = 200 * 1024 * 1024;
    iRet = p_init(size);
    if (iRet) {
        printf("Error in p_init!\n");
        return -1;
    }
    p_clear();

    FILE *fp;
    /* File operation */
    fp = fopen("redisdata.txt","w+");
    if (fp == NULL) {
        fprintf(stderr, "open file failed!\n");
        exit(EXIT_FAILURE);
    }

    /* Add a while loop here */
    while(1) {
    /* Generate random allocation size */
    rand_size_1 = rand() % 22 * 1024 * 1024;
    rand_size_2 = rand() % 20 * 1024 * 1024;
    rand_size_3 = rand() % 65 * 1024 * 1024;

    int rand_flag;

    // output check
    // printf("debug: rand_size_1 is %d.\n", rand_size_1);
    // printf("debug: rand_size_2 is %d.\n", rand_size_2);
    // printf("debug: rand_size_3 is %d.\n", rand_size_3);

    /* allocation process */
    int *obj_1 = (int *)p_malloc(ID_1, rand_size_1);
    file_record(fp);

    int *obj_2 = (int *)p_malloc(ID_2, rand_size_2);
    file_record(fp);

    int *obj_3 = (int *)p_malloc(ID_3, rand_size_3);
    file_record(fp);

    /* recycling process */

    p_free(ID_3);
    file_record(fp);

    p_free(ID_2);
    file_record(fp);

    p_free(ID_1);

    p_clear();
    // file_record(fp);
    }
    fclose(fp);

    return 0;
}

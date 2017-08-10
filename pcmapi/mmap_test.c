/*************************************************************************
	> File Name: mmap_test.c
	> Author: 
	> Mail: 
	> Created Time: 2017年06月07日 星期三 21时08分02秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<sys/mman.h>
#include<fcntl.h>
#include<unistd.h>
#include<time.h>

#define FILEPATH_R "./ramdisk/mmap.bin"
#define FILEPATH_D "./disk/mmap.bin"
#define NUMMAX (1000 * 10000)
#define FILESIZE (NUMMAX * sizeof(int))
int main(int argc, char *argv[]) {
    int i;
    int fd;
    int result;
    int *map;
    double duration;
    float throuput;
    clock_t start, end;
    clock_t in_start, in_end;
    int rand_loc = 1;
    char control = argv[1][0];
    int NUMINTS = 1000;

    srand((unsigned int)time(NULL));

    start = clock();
    if (control == 'R') {
        fd = open(FILEPATH_R, O_RDWR|O_CREAT|O_TRUNC,(mode_t)0600);
    } else {
        fd = open(FILEPATH_D, O_RDWR|O_CREAT|O_TRUNC,(mode_t)0600);
    }
    if (fd == -1) {
        perror("Error opening a file for writing");
        exit(EXIT_FAILURE);
    }
    end = clock();
    duration = (double)(end-start) / CLOCKS_PER_SEC;
    printf("File opening time: %f seconds\n", duration);

    result = lseek(fd, FILESIZE-1,SEEK_SET);
    if (result == -1) {
        close(fd);
        perror("Error calling lseek() to 'stretch' the file");
        exit(EXIT_FAILURE);
    }

    result = write(fd, "", 1);
    if (result != 1) {
        close(fd);
        perror("Error writing last byte of the file");
        exit(EXIT_FAILURE);
    }
    
    start = clock();
    map = (int *)mmap(0, FILESIZE, PROT_READ|PROT_WRITE, MAP_SHARED, fd, 0);
    if (map == MAP_FAILED) {
        close(fd);
        perror("Error mapping the file");
        exit(EXIT_FAILURE);
    }
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("Mmap time: %f seconds\n", duration);
    
    int *map_addr = map;
    int gran = 1;
    for (; NUMINTS <= NUMMAX; NUMINTS = NUMINTS * 10) {
        start = clock();
        for (i = 0;i < NUMINTS;i++) {
            rand_loc = (int)((double)rand() / (RAND_MAX) * (NUMINTS-2) / 2) * 2 + 1;
            map[rand_loc] = i; // key
            map[rand_loc+1] = 2 * i; // value
            map_addr = map + rand_loc;
            if (i % gran == 0) {
                result = msync(map_addr, sizeof(int)*gran*2, MS_ASYNC);
            }
        }
        end = clock();
        duration = (double)(end-start)/CLOCKS_PER_SEC;
        throuput = NUMINTS / duration; 
        printf("-----------------------------------------------------------------------------\n");
        printf("data number: %10d | time delay: %10f(ms) | throuput: %10f(Ops/Sec)\n", NUMINTS, duration*1000, throuput);
        printf("-----------------------------------------------------------------------------\n\n");
    }

    if (munmap(map,FILESIZE) == -1) {
        perror("Error unmapping the file");
        close(fd);
        exit(EXIT_FAILURE);
    }

    start = clock();
    close(fd);
    end = clock();
    duration = (double)(end-start)/CLOCKS_PER_SEC;
    printf("File close time: %f seconds\n", duration);

    return 0;
}

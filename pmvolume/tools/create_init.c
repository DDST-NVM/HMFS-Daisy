#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
	FILE *fp = fopen("init_tmp", "w");
	if(fp == NULL)
		return -1;
	int startIdx, count, i;

	if(argc == 1) {
		startIdx = 0;
		count = 3000;
	} else if(argc == 3) {
		startIdx = atoi(argv[1]);	
		count = atoi(argv[2]);
	}

	for(i=startIdx; i<count+startIdx; ++i) {
		fprintf(fp, "%d 0 32\n", i);
	}
	fclose(fp);
	return 0;
}


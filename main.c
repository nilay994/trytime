#include <sys/time.h>
#include <stdio.h>
#include <unistd.h>

long getMicrotime(){
	struct timeval currentTime;
	gettimeofday(&currentTime, NULL);
	return currentTime.tv_sec * (int)1e6 + currentTime.tv_usec;
}

void main() {
	long int start = getMicrotime();
	sleep(0.40);
	long int end = getMicrotime();
	printf("start: %d\n", start);
	printf("end: %d\n", end);
	printf("diff: %d\n", end-start);
}

#include<stdio.h>
#include<math.h>

void main() {
	int sum = 234;
	int digit = 0;

	for (int i = 0; i<3; i++) {
		int rem = (int) pow(10, i+1);
		printf("rem: %d\n", rem);
		digit = (sum % rem)/ pow(10, i);
		printf("digit %d: %d \n", i, digit);
	}

}

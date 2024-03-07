#include <stdint.h>
#include <stdio.h>

void main()
{
	int8_t a = 127;
	a = a + 2;
	printf("Positive signed number overflow, 127 + 2: %d\n", a);

	a = -128;
	a = a - 1;
	printf("Negative signed number overflow, -128 - 1: %d\n", a);
}


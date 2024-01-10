#include <stdio.h>
#include <stdint.h>

int main() {
	uint8_t name_of_var = 5u;
	name_of_var ^= 6u;
	printf("%d\n", name_of_var);
	return 0;
}


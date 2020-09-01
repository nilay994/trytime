#include <stdio.h>
#include <stdint.h>

void printnum(int8_t num) {
	printf("%d, 0x%02x\n", num, num);
}


void main() {
	int8_t num = 50;
	printnum(num);

	int8_t rightshifted = (int8_t) num >> 1;
	int8_t leftshifted = (int8_t) num << 1;

	printnum(rightshifted);
	printnum(leftshifted);

	int8_t twos_cmpl = ~num + 1;
	printnum(twos_cmpl);

	int8_t right_cmpl_shift = (int8_t) twos_cmpl >> 1;
	printnum(right_cmpl_shift);

}

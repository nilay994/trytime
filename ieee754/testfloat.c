#include <stdio.h>
#include <string.h>
#include <stdint.h>
// #include "ieee754.h"
// test using: https://www.h-schmidt.net/FloatConverter/IEEE754.html

/* convert float into 4 bytes 
inline uint32_t pack754_32(float f) {
	uint32_t float2int
} */

int main() {

	float num_float = -0.00005;
	uint32_t num_bits;
	memcpy(&num_bits, &num_float, sizeof(num_float));
	printf("float: %f, hex: 0x%08x\n", num_float, num_bits);

	float num_float2;
	uint32_t num_bits2 = 0xc8650bd3; 
	memcpy(&num_float2, &num_bits2, sizeof(uint32_t));
	printf("float2: %f, hex2: 0x%08x\n", num_float2, num_bits2);
	return 0;
}

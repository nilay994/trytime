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

	float num_float = -0.005;
	// uint32_t num_bits;
	uint8_t num_bits[10] = {0};
	memcpy(&num_bits[1], &num_float, sizeof(num_float));
	printf("float: %f, hex: %02x%02x%02x%02x\n", num_float, num_bits[3], num_bits[2], num_bits[1], num_bits[0]);

	float num_float2;
	uint32_t num_bits2 = 0xc8650bd3; 
	memcpy(&num_float2, &num_bits2, sizeof(uint32_t));
	printf("float2: %f, hex2: 0x%08x\n", num_float2, num_bits2); 

	return 0;
}

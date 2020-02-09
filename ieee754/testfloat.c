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

	float num_float = -1.234;
	// uint32_t num_bits;
	uint8_t num_bits[10] = {0};
	memcpy(&num_bits[4], &num_float, sizeof(num_float));
	// 7 is MSB
	printf("float: %f, hex: %02x%02x%02x%02x\n", num_float, num_bits[7], num_bits[6], num_bits[5], num_bits[4]);

	float num_float2;
	uint8_t num_bits2[4] = {0xe1, 0x3a, 0x6a, 0xc3};  
	memcpy(&num_float2, &num_bits2, sizeof(uint32_t));
	printf("float2: %f, hex2: 0x%08x\n", num_float2, num_bits2[0]); 

	return 0;
}

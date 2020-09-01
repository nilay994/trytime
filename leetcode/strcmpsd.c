#include <stdio.h>
#include <string.h>
#include <stdint.h>

int main () {
   uint8_t str1[15];
   uint8_t str2[15];
   int ret;

   strcpy(str1, "abcdef");
   strcpy(str2, "abcdef");

   ret = strcmp((const char*)str1, (const char *)str2);

   if(ret < 0) {
      printf("str1 is less than str2");
   } else if(ret > 0) {
      printf("str2 is less than str1");
   } else {
      printf("str1 is equal to str2");
   }

	uint8_t nval_hex[4] = {0};
	float nval = 135.0;
	memcpy(&nval_hex, &nval, sizeof(float));
	printf("\n----nvalhex\n");
	for(int i =0; i<4; i++){
		printf("0x%02x,", nval_hex[i]);
	}

	uint8_t hex2f_test[4] = {0x00,0x00,0x07,0x43};
	float valf;
	memcpy(&valf, &hex2f_test, 4);
	printf("\nvalf: %f\n", valf);
	char s = '*';
	printf("* hex: 0x%02x, %d\n", s, s);

	uint8_t aer = 178;
	printf("aer: %c\n", aer);
	 return(0);


}

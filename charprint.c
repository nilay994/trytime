#include <stdio.h>
#include <string.h>
#include <math.h>

#include <stdint.h>

void strptrtest(uint8_t* strs) {
	char* teststr = "mavlab test\n";
	memcpy(strs, teststr, sizeof(teststr));
}


int main() {

	/*
	char str[23];
	int num = 42;
	sprintf(str, "%d", num);

	// predict length of string based on integer
	int strlen = (int)((ceil(log10(num))+1)*sizeof(char));


	char strnew[50] = {0};
	memset(strnew, 'a', 49);
	printf("strnew before: %s\n", strnew);
	// if number = 42.53453, %06.2f indicates total 6 including commas and post-decimal places
	// 6 characters = 042.53 (one char for decimal)
	char north_str[7] = {0};
	sprintf(north_str, "%06.2f", 42.53453);
	int loc = strncpy(&strnew[10], north_str, 6);
	printf("strnew after: %s, %c\n", strnew, &loc);


	char ch[10];
	ch[0] = 'a';
	ch[1] = '\0';
	printf("str: %s\n", ch);
	char str1[10] = {'\0'};
	str1[0] = 'r';
	str1[1] = 'n';
	str1[2] = 'a';
	// printf("strlen: %ld\n", strlen(str1));
	char *str2 = "hey";
	// strncpy(str1, str2, strlen(str2));
	// printf("str1:%s, str2:%s\n", str1, str2);

	char stringtry[40] = {'a'};
	printf("herhehere: %s\n", stringtry);

	char eta[4] = {'0'};
	sprintf(eta, "%03d", 99);
	printf("%s\n", eta);
	
	char drone[2][15] = {'0'};
	char strtrs[15] = "abcdefghijklmn";
	printf("strtrs: %s\n", strtrs);

	drone[1][15] = 0;
	// memset(drone[1], 'b', 14);
	char north_str[7] = {'0'};

	printf("drone0: %s\n", drone[0]);
	printf("drone1: %s\n", drone[1]);
	strncpy(north_str, &strtrs[8], 6);
	printf("north_str: %s\n", north_str);
	

	char str1[10] = {'0'};
	memset(str1, 'a', 10);
	str1[9] = 0;

	char str2[3];
	memset(str2, 'b', 3);

	printf("before: str1: %s, str2: %s\n", str1, str2);

	strncpy(&str1[3], str2, 3);
	printf("after: str1: %s, str2: %s\n", str1, str2);


	float temp_var = strtof("-1.5342", NULL);
	printf("temp_var orig: %f\n", temp_var);

	char tmp_var_str[6] = {0};
	
	// The generated string has a length of at most n-1, 7-1 = 6 here and %06.2f means total of 6 characters from which two are decimals 
	snprintf(tmp_var_str, 7, "%06.2f", temp_var);

	printf("after snprintf: %s\n", tmp_var_str);

	*/

	uint8_t mode = 0xAA;
	// char tmp = '0' + mode;
	printf("%x", mode);

	uint8_t str[50] = {0};
	strptrtest(str);
	printf("\n str: %s\n", str);
	return 0;
}

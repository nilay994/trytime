#include <stdio.h>
#include <stdint.h>

/* Make a pointer to a matrix of _rows lines
   converts float mat(*)[10] to **mat...    */
#define MAKE_MATRIX_PTR(_ptr, _mat, _rows) \
  float * _ptr[_rows]; \
  { \
    int __i; \
    for (__i = 0; __i < _rows; __i++) { _ptr[__i] = &_mat[__i][0]; } \
  }


void foo(float **mat) {
	mat[4][4] = 4;
}

void main() {
	float _arr[5][5] = {0};
	MAKE_MATRIX_PTR(arr, _arr, 5);
	foo(arr);
	for(int i=0; i<5; i++) {
		for(int j=0; j<5; j++) {
			printf("%3.2f, ", _arr[i][j]);
		}
	printf("\n");
	}
	printf("arr 4,4: %f\n", _arr[4][4]);
}

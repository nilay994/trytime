#include <stdio.h>

/** Make a pointer to a matrix of _rows lines */
#define MAKE_MATRIX_PTR(_ptr, _mat, _rows) \
  float * _ptr[_rows]; \
  { \
    int __i; \
    for (__i = 0; __i < _rows; __i++) { _ptr[__i] = &_mat[__i][0]; } \
  }



// TODO: check void to float typecast (works)
// check dereference (works)
// check pprz make matrix ptr (added above)

void print_matrix(void **a_ptr, int m, int n) {
	static int cnt = 1;
	printf("******* Matrix%d: (%d, %d) *********\n", cnt, m, n);
	cnt ++;
	// typecast void ptr to float mat
	float **mat = (float **) a_ptr;

	for (int i = 0; i < m; i++) {
		// address of the 1st element of the row 
		// printf("0x%02x:", (unsigned int)(& mat[i][0]));
		for (int j = 0; j < n; j++) {
			// find address of elements, typecast to pointer datatype (unsigned int)
			// print only LSB of memory location
			printf("0x%hhx\t", (unsigned int)(& mat[i][j]));
		}
		printf("\n");
	}
	printf("************\n");
}


void main() {

	float a[3][2] = {{1, 2}, {3, 4}, {5, 6}};

	float *ptr_a[3];
	for (int i = 0; i < 3; i++) {
		printf("store row: 0x%02x\n", &a[i][0]);
		ptr_a[i] = &a[i][0];
	}
	print_matrix((void**) ptr_a, 3, 2);

	float b[4][4];
	b[0][0] = 2;
	float *ptr_b[4];
	for (int i = 0; i < 4; i++) {
		ptr_b[i] = &b[i][0];
	}
	print_matrix((void**) ptr_b, 4, 4);

	float c[4][4];
	c[0][0] = 3;
	// float *ptr_c[4];
	// for (int i = 0; i < 4; i++) {
	// 	ptr_c[i] = &c[i][0];
	// }
	// print_matrix((void**) ptr_c, 4, 4);

	// can't copy by reference
	void **ptr_c = ptr_a;
	print_matrix(ptr_c, 3, 2);

	// can't swap a and b;
	// ptr_c = ptr_a;
	// ptr_a = ptr_b;
	// ptr_b = ptr_c;

	// print after swapping
	// print_matrix(a_ptr, 4, 4);
	// print_matrix(b_ptr, 4, 4);
	// print_matrix(c_ptr, 4, 4);

}

#include <stdio.h>

// error: this is a smart way of passing by value, but still passing by reference.
// Does not overwrite any variables.
void foo1(const int *arg) {
	*arg = *arg + 1;
	printf("arg: %d\n", *arg);
}

// allowed in g++ but not in gcc
void foo(int& arg) {
	arg = arg + 1;
	printf("arg: %d\n", arg);
}

int main(void*) {
	int a = 101;
	foo(a);
	foo1(&a);
	return 0;
}

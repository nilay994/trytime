/* friend function, virtual override, break in nested loop */

#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

class A {
    public:
        int vara = 5;
        // let the derived class override printvar..
        virtual void printvar() {
                std::cout << this->vara << std::endl;
        };        
};

class B: public A {
    public:
        int varb = 10;
        void printvar() {
            std::cout << varb << std::endl;
        };
        friend void outside_fn(B);
};

void outside_fn(B b) {
    std::cout << "Outside fn a.var: " << b.varb << std::endl;
}

// todo: sqrt, palindromes, reinterpret_cast(rpg code)
// https://www.javatpoint.com/spacex-interview-questions


void prime_test(int n) {
    bool true_flag = 0;
    int cnt_inner = 1;
    int cnt_outer = 1;

    // nested loop break test, check for 10 times; 
    // break takes you only out of 1 while loop, not nested loops
    while (cnt_outer < 50) {
        while (cnt_inner < 50) {
            for (int i = 2; i < n; i ++) {
                if (n % i == 0) {
                    std::cout<< "not prime" << std::endl;
                    true_flag = 1;
                    break;
                }
            } 
            if (!true_flag) {
                std::cout<< "prime" << std::endl;
                break;
            }
            std::cout<< "cnt_inner: " << cnt_inner << std::endl;
            cnt_inner ++;
        }
        std::cout<< "cnt_outer: " << cnt_outer << std::endl;
        cnt_outer ++;
    }

}

// memory: heap and stack both are on ram, you can even store files in /dev/shm...
// heap is memory alloc during runtime: we should worry about malloc, calloc (malloc with zero) and free.. new() and delete().., 
// slower access, not in cache but maybe somewhere in the ram... 
// stack is memory alloc during compile time: faster access, maybe higher parts of ram + reusing a lot puts you in L1 cache.. 
// to be even faster than stack, "register" keyword stores in register rather than on a stack

int main() {
    A* a;
    B b;
    // // a can't access anything of b, it is a baseclass
    // a->printvar();
    // // b can access private of a... but to use the virtual function, object of a must point to instance of b,
    a = &b;
    a->printvar();
    outside_fn(b);
    prime_test(17);

    // ptr test
    int x = 67;
    int *m = &x;
    int *n = m;
    // char *t = n;
    printf("%d %d %d\n\n", n, *n, m);

    // printf returns how many bytes/chars it printed..

    int arr[] = {10, 20, 30, 40, 50, 60};
    int *p= (int*)(&arr+1);
    printf("%d", *(p-1)); 

    return 0;
}
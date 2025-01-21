#include <iostream>

typedef struct {
    int a;
    float b;
} eg_struct;

eg_struct eg1 = {
    .a = 5,
    .b = 4.5f
};

int main()
{
    std::cout << eg1.b << std::endl;
    return 0;
}
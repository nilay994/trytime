#include <iostream>

void fn(int a, int b = 0, float c = 0)
{
    std::cout << "a: " << a << std::endl;
    std::cout << "b: " << b << std::endl;
    std::cout << "c: " << c << std::endl;
}


int main()
{
    fn(0, 4, 0);
    fn(0, 3.0f);
    return 0;
}
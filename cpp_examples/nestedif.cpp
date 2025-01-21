#include <iostream>

int main() {
    int val = 5;
    if (true)
    {
        if (val == 5) {
            std::cout << "in" << std::endl;
            break;
        }
    } else
    {
        std::cout << "else" << std::endl;
    }
}
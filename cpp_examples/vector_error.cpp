#include <iostream>
#include <vector>

int main()
{
    std::vector<int> a = {1, {2, 3}};

    for (auto it : a) {
        std::cout << it << std::endl;
    }
    return 0;
}
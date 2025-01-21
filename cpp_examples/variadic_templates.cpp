#include <iostream>
#include <vector>
#include <string>

template <typename... T1, typename T2>
void fn (T1... st, T2 val) {
    
    std::vector <std::string> sts = {st...};
    for (auto ele : sts) {
        std::cout << "string: " << ele << std::endl;
    }
    int local_val = val + 1;
    (void) local_val;
}


int main()
{
    fn("st1", "st2", "st3", 4);
    return 0;
}
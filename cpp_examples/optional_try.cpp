#include <iostream>
#include <optional>


std::optional<double> GetLimit(double val)
{
    if (val < 40) {
        return {};
    } else {
        return val;
    }
}


int main()
{
    std::optional<double> ret =  GetLimit(54.0);
    std::cout << "out: " << ret.value_or(5.0) << std::endl;
    return 0;
}

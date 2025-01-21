#include <iostream>
#include "inline_share.h"

int main()
{
    priv::blah += "blah";
    std::cout << priv::blah << std::endl;
    return 0;
}
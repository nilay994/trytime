#include <iostream>
#include "inline_share.h"
#include "twice_include.h"

void foo()
{
    std::cout << priv::blah << std::endl;
}
from CodeGen import *

cpp = CppFile("test.cpp")
cpp("#include <iostream>")

with cpp.block("void main()"):
    for i in range(5):
        cpp('std::cout << "' + str(i) + '" << std::endl;')

cpp.close()

/**
 * check how commas, spaces and decimals are resolved in ss class
 */ 

#include <vector>
#include <string>
#include <sstream>
#include <iostream>

int main()
{
    std::string str = "34  0.1,45,12.3,100,34.6,50";
    std::vector<float> vect;

    std::stringstream ss(str);

    float i;

    std::cout << ss.str() << std::endl;

    while (ss >> i) {
        vect.push_back(i);
        if ((ss.peek() == ',') || (ss.peek() == ' ')) {
            std::cout << ss.str() << std::endl;
            std::cout << "ignoring char " << (char) ss.peek() << std::endl;
            ss.ignore();
        }
    }

    for (i=0; i< vect.size(); i++)
        std::cout << vect.at(i)<<std::endl;
}
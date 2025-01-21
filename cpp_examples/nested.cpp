#include <iostream>
#include <string>

class ROV
{
    ROV(const std::string& str) { val = str; }

 private:
    std::string val;
};

class Fugro
{
    ROV rov;

    Fugro(const std::string& str)
        : rov(str)
    {
    }

    std::string toString() const { return "fugro/" + rov.toString(); }
};

// Example usage
int main()
{
    try
    {
        // could it be as simple as this below? separate topic names into commas and for each
        // arg, just hardcode the possibilites.
        Fugro topic("apple", "nest1", "nest2");

        std::cout << topic.toString() << std::endl; // Output: fugro/rov/ui/apple/command
    }
    catch (const std::invalid_argument& e)
    {
        std::cerr << e.what() << std::endl;
    }

    return 0;
}
#include <iostream>
#include <map>

int main()
{
    std::map <int, char> intcharmap;
    intcharmap.try_emplace(5);
    intcharmap[5] = 'f';

    intcharmap.try_emplace(6);
    intcharmap[6] = 's';

    for (const auto& [number, character] : intcharmap)
    {
        if (number == 7) {
            intcharmap.erase(number);
        }
    }
    return 0;
}
#include <iostream>
#include <set>

template<typename T>
std::ostream& writecsv(const T& t, std::ostream& o)
{
    int i = 0;
    for(std::set<int>::const_iterator it = t.begin(); it != t.end(); it++)
    {
        if(i++ > 0) std::cout << ", ";
        std::cout << *it;
    }
    return o;
}

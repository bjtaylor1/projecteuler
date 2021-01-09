#include "primes.h"
#include <string>
#include <stdexcept>
#include <iostream>

int main(int argc, char** argv)
{
    if(argc < 2) throw std::runtime_error("Usage: primes max_ex");
    int max_ex = std::stoi(argv[1]);
    std::set<int> primes = makeprimes(max_ex);
    for(std::set<int>::const_iterator it = primes.begin(); it != primes.end(); it++)
    {
        std::cout << (*it) << std::endl;
    }
    return 0;
}
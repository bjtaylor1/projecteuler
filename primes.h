#pragma once
#include <set>
#include <cstring>

void primeseive(int max_ex, bool* isprime)
{
    memset(isprime, 1, (max_ex)*sizeof(bool));
    for(int i = 2; i < max_ex; i++ )
    {
        if(isprime[i])
        {
            for(int multiple = 2*i; multiple < max_ex; multiple += i)
            {
                isprime[multiple] = false;
            }
        }
    }
}

std::set<int> makeprimes(int max_ex, bool* isprime) // if we want to keep the seive
{
    std::set<int> returnvalue;
    primeseive(max_ex, isprime);
    for(int i = 2; i< max_ex; i++)
    {
        if(isprime[i]) returnvalue.insert(i);
    }
    return returnvalue;    
}

std::set<int> makeprimes(int max_ex)
{
    bool* isprime = new bool[max_ex];
    std::set<int> returnvalue = makeprimes(max_ex, isprime);
    delete[] isprime;
    return returnvalue;
}
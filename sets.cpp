#include <set>
#include <vector>
#include <iostream>
#include <algorithm>
#include <mpirxx.h>

void makesetcountslow(std::set<std::vector<int> >& all, std::vector<int>& current, int n, int k)
{
    if(current.size() == n)
    {
        std::set<int> distinct;
        distinct.insert(current.begin(), current.end());
        if(/*distinct.size() == k && */ all.insert(current).second)
        {
            int t = 0;
            for(auto v : current) 
            {
                std::cout << v << " ";
                if(v == k) t++;
            }
            std::cout << " D" << distinct.size(); //D = num distinct
            std::cout << " T" << *std::max_element(distinct.begin(), distinct.end()); //T = count of highest number
            std::cout << std::endl;
        }
    }
    else
    {
        for(int j = 1; j <= k; j++)
        {
            current.push_back(j);
            makesetcountslow(all, current, n, k);
            current.pop_back();
        }
    }
}

void setcountslow(int n, int k)
{
    std::set<std::vector<int> > all;
    std::vector<int> current;
    makesetcountslow(all, current, n, k);
}

int main(int argc, char** argv)
{
    int n = std::stoi(argv[1]);
    int k = std::stoi(argv[2]);
    setcountslow(n,k);
}
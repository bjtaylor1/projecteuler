#include <map>
#include <mpirxx.h>
#include <ostream>

template<typename T>
class counter : public std::map<T, int>
{
public:
    int total, distinct;
    counter() : total(0), distinct(0){}

    void add(const T& item)
    {
        typename std::map<T,int>::iterator existing = this->find(item);
        if(existing == this->end())
        {
            this->insert(std::pair<T, int>(item, 1));
            distinct++;
        }
        else
        {
            existing->second++;
        }
        total++;
    }
    void remove(const T& item)
    {
        typename std::map<T, int>::iterator it = this->find(item);
        if(it == this->end() || it->second == 0)
        {
            std::cerr << "counter::remove, not found " << item << std::endl;
            throw std::runtime_error("Error in counter::remove");
        }
        else if(it->second == 1)
        {
            this->erase(it);
            distinct--;
        }
        else
        {
            it->second--;
        }
        total--;
    }

    mpz_class get_permutations() const
    {
        mpz_class result;
        mpz_fac_ui(result.get_mpz_t(), total);
        for(typename std::map<T, int>::const_iterator it = this->begin(); it != this->end(); it++)
        {
            if(it->second > 1)
            {
                mpz_class repeat;
                mpz_fac_ui(repeat.get_mpz_t(), it->second);
                mpz_class oldresult(result);
                mpz_divexact(result.get_mpz_t(), oldresult.get_mpz_t(), repeat.get_mpz_t());
            }
        }
        return result;
    }

    std::ostream& write(std::ostream& os)
    {
        int i = 0;
        for(typename std::map<T, int>::const_iterator it = this->begin(); it != this->end(); it++)
        {
            if(i++ > 0) os << ", ";
            os << it->first << "x" << it->second;
        }
        return os;
    }

    std::ostream& writeverbose(std::ostream& os)
    {
        int i = 0;
        for(typename std::map<T, int>::const_iterator it = this->begin(); it != this->end(); it++)
        {
            for(int r = 0; r < it->second; r++)
            {
                if(i++ > 0) os << ", ";
                os << it->first;
            }
        }
        return os;
    }

};
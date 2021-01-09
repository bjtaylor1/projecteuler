#include <map>

template<typename TKey, typename TVal>
class cache
{
public:
    std::map<TKey, TVal> entries;

    template<typename F>
    TVal get_or_add(const TKey& key, F generator)
    {
        typename std::map<TKey, TVal>::const_iterator it = entries.find(key);
        if(it == entries.end())
        {
            std::cout << "Cache miss" <<std::endl;
            TVal val = generator(key);
            entries.insert(typename std::pair<TKey, TVal>(key, val));
            return val;
        }
        else
        {
            std::cout << "Cache hit" <<std::endl;
            return it->second;
        }
    }
};
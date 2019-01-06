#pragma once

#ifndef THREADED_AGGREGATOR
#define THREADED_AGGREGATOR
#include "stdafx.h"

template<class TRes, class TParam>
class threadedaggregator
{
private:
    const int numThreads = 4;
    std::mutex mutex;
public:
    void calculate_component_internal(const TParam& lowerboundinc, const TParam& upperboundexc)
    {
        TRes component = calculate_component(lowerboundinc, upperboundexc);
        std::lock_guard<std::mutex> lock(mutex);
        result += component;
    }
    TRes result;
    virtual TRes calculate_component(const TParam& lowerboundinc, const TParam& upperboundexc) = 0;
    
    void calculate(const TParam& lowerboundinc, const TParam& upperboundexc)
    {
        std::vector<std::thread> threads;
        TParam count_per_thread = upperboundexc - lowerboundinc;
        TParam threadlowerboundinc = lowerboundinc;
        for (TParam  threadupperboundexc = threadlowerboundinc + count_per_thread;
            threadupperboundexc <= upperboundexc;
            threadupperboundexc = threadlowerboundinc + count_per_thread)
        {
            std::thread t(&threadedaggregator::calculate_component, this, lowerboundinc, upperboundexc);
            threads.push_back(t);
            threadlowerboundinc = threadupperboundexc;
        }

        for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); it++)
        {
            it->join();
        }
    }
    virtual ~threadedaggregator() {}
};


#endif
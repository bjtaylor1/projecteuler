#pragma once

#ifndef THREADED_AGGREGATOR
#define THREADED_AGGREGATOR
#include "stdafx.h"

template<class TRes, class TParam>
class threadedaggregator
{
private:
    const int numThreads = 5;
    std::mutex mutex;
    void calculate_component_internal(const TParam& lowerboundinc, const TParam& upperboundexc)
    {
        TRes component = calculate_component(lowerboundinc, upperboundexc);
        std::lock_guard<std::mutex> lock(mutex);
        result += component;
    }
public:
    threadedaggregator() : result(0) {}
    TRes result;
    virtual TRes calculate_component(const TParam& lowerboundinc, const TParam& upperboundexc) = 0;
    
    void calculate(const TParam& lowerboundinc, const TParam& upperboundexc)
    {
        std::vector<std::thread*> threads;
        TParam count_per_thread = (upperboundexc - lowerboundinc) / numThreads;
        TParam threadlowerboundinc = lowerboundinc;
        for (TParam  threadupperboundexc = threadlowerboundinc + count_per_thread;
            threadupperboundexc <= upperboundexc;
            threadupperboundexc = threadlowerboundinc + count_per_thread)
        {
            if (threadupperboundexc > upperboundexc) threadupperboundexc = upperboundexc;
            std::thread* t = new std::thread(&threadedaggregator::calculate_component_internal, this, threadlowerboundinc, threadupperboundexc);
            threads.push_back(t);
            threadlowerboundinc = threadupperboundexc;
        }

        for (auto thread : threads)
        {
            thread->join();
            delete thread;
        }
    }
    virtual ~threadedaggregator() {}
};


#endif
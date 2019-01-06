#pragma once

#ifndef THREADED_AGGREGATOR
#define THREADED_AGGREGATOR
#include "stdafx.h"

template<class TRes, class TParam>
class threadedaggregator
{
private:
    const int numThreads = 32;
public:
    virtual TRes calculate_component(const TParam& lowerboundinc, const TParam& upperboundexc) = 0;
    TRes calculate(const TParam& lowerboundinc, const TParam& upperboundexc)
    {
        std::thread t(&threadedaggregator::calculate_component, this, lowerboundinc, upperboundexc);
        t.join();
        return TRes();
    }
    virtual ~threadedaggregator() {}
};


#endif
#pragma once
#ifndef THREADHELPER
#define THREADHELPER

#include <thread>
#include <atomic>
#include <thread>
#include <vector>

template<class TRes, class TParam, class TFunc>
TRes parallel_sum(TParam fromInc, TParam toExc, TFunc func)
{
	std::atomic<TRes> res(0);
	std::vector<std::thread> threads;
	const int threadcount = 16;
	const int revisits = 10; //average number of times each thread will have to loop. counts for the fact some ranges might involve more work than others
	TParam batchSize = (toExc - fromInc) / (threadcount * revisits);
	if (batchSize <= 0) batchSize = 1;
	std::atomic<TParam> currentstart = fromInc;
	for (long i = 0; i < threadcount; i++)
	{
		threads.push_back(std::thread([batchSize, func, toExc](std::atomic<TParam>& currentstart, std::atomic<TRes>& res)
		{	
			for (TParam start = currentstart.fetch_add(batchSize);
				start < toExc;
				start = currentstart.fetch_add(batchSize))
			{
				TParam end = start + batchSize;
				if (end > toExc) end = toExc;
				TRes thisRes = func(start, end);
				res += thisRes;
			}
		}, std::ref(currentstart), std::ref(res)));
	}
	for (std::vector<std::thread>::iterator it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}
	return (TRes)res;
}

#endif
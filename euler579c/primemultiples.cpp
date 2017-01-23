#include "stdafx.h"
#include "primemultiples.h"

mutex primemultiples::m;

set<primemultiple> primemultiples::operator[](long long total) const
{
	lock_guard<mutex> lm(m);
	auto item = mem.find(total);
	return item == mem.end() ? set<primemultiple>() : item->second;
}


void primemultiples::add(long long total, long long prime, const mnpq& item)
{
	lock_guard<mutex> lm(m);
	auto pairs = mem.insert(pair<long long, set<primemultiple>>(total, set<primemultiple>()));
	pairs.first->second.insert(primemultiple(prime, item));
}
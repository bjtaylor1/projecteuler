#pragma once

#ifndef CLASS_PRIMEMULTIPLES
#define CLASS_PRIMEMULTIPLES
#include "primemultiple.h"


class primemultiples
{
private:
	static mutex m;
	map<long long, set<primemultiple>> mem;

public:
	set<primemultiple> operator[](long long total) const;
	void add(long long total, long long prime, const mnpq& item);
};

#endif
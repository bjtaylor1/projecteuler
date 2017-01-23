#pragma once
#ifndef CLASS_PRIMEMULTIPLE
#define CLASS_PRIMEMULTIPLE

#include "mnpq.h"

class primemultiple
{
public:
	long long primefactor;
	mnpq item;
	primemultiple(long long, const mnpq&);
};

bool operator<(const primemultiple&, const primemultiple&);

#endif
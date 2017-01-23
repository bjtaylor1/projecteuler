#include "stdafx.h"
#include "primemultiple.h"

primemultiple::primemultiple(long long PrimeFactor, const mnpq& Item) : primefactor(PrimeFactor), item(Item) {}

bool operator<(const primemultiple& lhs, const primemultiple& rhs)
{
	if (lhs.primefactor != rhs.primefactor) return lhs.primefactor < rhs.primefactor;
	if (lhs.item != rhs.item) return lhs.item < rhs.item;
	return false;
}

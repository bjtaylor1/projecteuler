#pragma once

template<class TIt, class Select>
TIt min_select(TIt first, TIt last, Select s)
{
	TIt smallest = first;
	++first;
	for (; first != last; ++first)
	{
		if (s(*first) < s(*smallest))
		{
			smallest = first;
		}
	}
	return smallest;
}
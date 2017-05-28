#pragma once
#ifndef CLASS_HEXWEIGHT
#define CLASS_HEXWEIGHT

class hexweight
{
public:
	long s0, s1, s2, s3, s4, s5;
	hexweight(long s0, long s1, long s2, long s3, long s4, long s5);
	bool operator<(const hexweight& rhs) const;
	std::vector<long> get_sides() const;
};

#endif

#pragma once
#ifndef CLASS_HEXAGON
#define CLASS_HEXAGON

class hexagon
{
public:
	std::vector<long> sides;
	long perimeter;
	hexagon(const std::vector<long>& _sides, long _perimeter);
	bool operator <(const hexagon& rhs) const;
};

#endif

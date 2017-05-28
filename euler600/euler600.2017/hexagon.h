#pragma once
#ifndef CLASS_HEXAGON
#define CLASS_HEXAGON

class hexagon
{
public:
	std::vector<long> sides, sides_orig;
	long perimeter;
	hexagon(const std::vector<long>& _sides, long _perimeter);
	bool operator <(const hexagon& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const hexagon& h);

#endif

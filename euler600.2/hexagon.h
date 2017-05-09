#pragma once
#ifndef CLASS_HEXAGON
#define CLASS_HEXAGON

class hexagon
{
public:
	std::vector<long> sides, sides_orig;
	hexagon(const std::vector<long>& _sides);
	bool operator <(const hexagon& rhs) const;
};

std::ostream& operator<<(std::ostream& os, const hexagon& h);

#endif

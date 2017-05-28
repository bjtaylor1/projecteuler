#include "stdafx.h"
#include "hexagon.h"
#include "hexweight.h"
using namespace std;


hexagon::hexagon(const vector<long>& s, long _perimeter) : perimeter (_perimeter), sides_orig(s)
{
	set<hexweight> hexweights;
	hexweights.insert(hexweight(s[0], s[1], s[2], s[3], s[4], s[5]));
	hexweights.insert(hexweight(s[1], s[2], s[3], s[4], s[5], s[0]));
	hexweights.insert(hexweight(s[2], s[3], s[4], s[5], s[0], s[1]));
	hexweights.insert(hexweight(s[3], s[4], s[5], s[0], s[1], s[2]));
	hexweights.insert(hexweight(s[4], s[5], s[0], s[1], s[2], s[3]));
	hexweights.insert(hexweight(s[5], s[0], s[1], s[2], s[3], s[4]));

	hexweights.insert(hexweight(s[5], s[4], s[3], s[2], s[1], s[0]));
	hexweights.insert(hexweight(s[4], s[3], s[2], s[1], s[0], s[5]));
	hexweights.insert(hexweight(s[3], s[2], s[1], s[0], s[5], s[4]));
	hexweights.insert(hexweight(s[2], s[1], s[0], s[5], s[4], s[3]));
	hexweights.insert(hexweight(s[1], s[0], s[5], s[4], s[3], s[2]));
	hexweights.insert(hexweight(s[0], s[5], s[4], s[3], s[2], s[1]));

	sides = hexweights.rbegin()->get_sides();
}

bool hexagon::operator<(const hexagon & rhs) const
{
	return sides < rhs.sides;
}

ostream& operator<<(ostream& os, const hexagon& h)
{
	for (auto s : h.sides_orig)
	{
		os << s << " ";
	}
	return os;
}

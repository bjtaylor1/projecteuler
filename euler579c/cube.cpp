#include "stdafx.h"
#include "cube.h"

using namespace std;

bool operator<(const cube & lhs, const cube & rhs)
{
	set<vector3d>::const_iterator it_l = lhs.uvn.begin(), it_r = rhs.uvn.begin();
	for (long l = 0; l < 3; l++)
	{
		if (it_l == lhs.uvn.end() || it_r == rhs.uvn.end()) throw exception("A cube has less than 3 (distinct) vectors");
		if (*it_l != *it_r) return *it_l < *it_r;
	}
	if (it_l != lhs.uvn.end() || it_r != rhs.uvn.end()) throw exception("A cube has more than 3 vectors");
	return false;
}

cube::cube(vector3d & U, vector3d & V, vector3d & N) : uvn({U,V,N})
{

}

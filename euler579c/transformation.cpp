#include "stdafx.h"
#include "transformation.h"

string get_desc(char Axis, double Angle)
{
	stringstream ss_desc;
	ss_desc << "Rotate " << Angle << " about " << Axis << " axis";
	return ss_desc.str();
}

transformation::transformation(const vector<long long>& Numbers, const string& Desc) : numbers(Numbers), desc(Desc)
{
	if (Numbers.size() != 9) throw runtime_error("A transformation must be initialized with 9 numbers.");
}

transformation::transformation(const vector<long long>& Numbers, char Axis, double Angle) : transformation(Numbers, get_desc(Axis, Angle)) {}



long long transformation::operator[](size_t i) const
{
	return numbers[i];
}

vector3d operator*(const transformation& t, const vector3d& v)
{
	long long 
		x = t[0] * v.x + t[1] * v.y + t[2] * v.z,
		y = t[3] * v.x + t[4] * v.y + t[5] * v.z,
		z = t[6] * v.x + t[7] * v.y + t[8] * v.z;
	return vector3d(x, y, z);
}


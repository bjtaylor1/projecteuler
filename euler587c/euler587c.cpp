// euler587c.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
using namespace std;

double get_y(double a, double b, double c, int sign)
{
	double y = (-b + sign*sqrt(b*b - 4 * a*c)) / (2 * a);
	return y;
}

double get_y(double n, int sign)
{
	double a = (1.0 + (n*n));
	double b = ((-2.0 * n) - 2.0 );
	double c = 1.0;
	double y = get_y(a, b, c, sign);
	return y;
}

static double areaCorner = 1 - (M_PI_4);

double get_area_concave_tri(long long n)
{
	double yA = get_y(n, -1),//we want the lowest value
		xA = yA * n,
		xB = 1.0,
		yB = xB / n,
		xC = 1,
		yC = 1;
	double areaTriangleABC = 0.5*((xA - xC) * (yB - yA) - (xA - xB) * (yC - yA)); //http://math.stackexchange.com/questions/516219/finding-out-the-area-of-a-triangle-if-the-coordinates-of-the-three-vertices-are
	double AH = 1 - xA;
	double angleBeta = asin(AH);
	double angleBetaDegrees = angleBeta * M_PI / 180;

	double areaArcACD = angleBeta / 2; // = (angleBeta / 2PI) * PI r^2
	double areaABD = areaArcACD - areaTriangleABC;
	if (areaABD <= 0) throw runtime_error("areaABD is zero or negative");
	double areaOBD = yB / 2;
	double areaConcave = areaOBD - areaABD;
	if (areaConcave <= 0) throw runtime_error("areaConcave is zero or negative");
	return areaConcave;
}

double get_proportion(long long n)
{
	double areaConcaveTri = get_area_concave_tri(n);
	double proportion = areaConcaveTri / areaCorner;
	return proportion;
}

int main()
{
	cout << get_proportion(1) << endl;
	cout << get_proportion(2) << endl;
	//cout << get_proportion(15) << endl;

    return 0;
}


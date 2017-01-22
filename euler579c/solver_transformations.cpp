#include "stdafx.h"
#include "solver.h"

transformation solver::t_x[] =
{
	make_x_transformation(0.0),
	make_x_transformation(90.0),
	make_x_transformation(180.0),
	make_x_transformation(270.0)
};
transformation solver::t_y[] =
{
	make_y_transformation(0.0),
	make_y_transformation(90.0),
	make_y_transformation(180.0),
	make_y_transformation(270.0)
};
transformation solver::t_z[] =
{
	make_z_transformation(0.0),
	make_z_transformation(90.0),
	make_z_transformation(180.0),
	make_z_transformation(270.0)
};

transformation solver::reflection[] =
{
	transformation(vector<long long>(
	{
		1,0,0,
		0,1,0,
		0,0,1
	}), "no-op (reflect)"),
	transformation(vector<long long>(
	{
		-1,0,0,
		0,1,0,
		0,0,1
	}), "reflect x"),
	transformation(vector<long long>(
	{
		1,0,0,
		0,-1,0,
		0,0,1
	}), "reflect y"),
	transformation(vector<long long>(
	{
		1,0,0,
		0,1,0,
		0,0,-1
	}), "reflect z")
};

transformation solver::make_x_transformation(double angleDegrees)
{
	long long sinval = (long long)(sin(angleDegrees * M_PI / 180));
	long long cosval = (long long)(cos(angleDegrees * M_PI / 180));
	return transformation(vector<long long>
	{
		1, 0, 0,
		0, cosval, -sinval,
		0, -sinval, cosval
	}, 'x', angleDegrees);
}
transformation solver::make_y_transformation(double angleDegrees)
{
	long long sinval = (long long)(sin(angleDegrees * M_PI / 180));
	long long cosval = (long long)(cos(angleDegrees * M_PI / 180));
	return transformation(vector<long long>
	{
		cosval, 0, sinval,
		0, 1, 0,
		-sinval, 0, cosval
	}, 'y', angleDegrees);
}
transformation solver::make_z_transformation(double angleDegrees)
{
	long long sinval = (long long)(sin(angleDegrees * M_PI / 180));
	long long cosval = (long long)(cos(angleDegrees * M_PI / 180));
	return transformation(vector<long long>
	{
		cosval, -sinval, 0,
		sinval, cosval, 0,
		0, 0, 1
	}, 'z', angleDegrees);
}
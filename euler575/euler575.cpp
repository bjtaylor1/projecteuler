// euler575.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

#define SIZE 5
#define COUNT (SIZE*SIZE)
class square
{
public:
	//bool favourCentre;
	int sideToCorner;
	int cornerStays;
	int sideToSide;
	int sideStays;
	int cornerToSide;
	int sideToMiddle;
	int middleToSide;
	int middleToMiddle;
	int middleStays;
	int corners, middles, sides;
	double corner, middle, side;


	square() 
	{
		//transitions:
		sideToCorner = 8;
		//no corner to corner, or middle to corner

		sideToSide = (((SIZE - 4) * 2) + 2) * 4; //per side: e.g. 1,2,3,4,5 - only 1 (3) has two adjacent sides, and 2 (2 and 4) have one adjacent side
		cornerToSide = 8;
		middleToSide = (SIZE - 2) * 4;
		sideToMiddle = middleToSide;
		middleToMiddle = ((SIZE - 4) * 3 + 2 + pow(SIZE - 4, 2)) * 4;

		corners = 4;
		sides = ((SIZE - 2) * 4);
		middles = COUNT - sides - corners;

		middleStays = middles;
		sideStays = sides;
		cornerStays = corners;

		corner = ((double)1) / COUNT;
		side = ((double)1) / COUNT;
		middle = ((double)1) / COUNT;

		int checksum1 = cornerStays + cornerToSide +
			sideStays + sideToCorner + sideToSide + sideToMiddle +
			middleStays + middleToSide + middleToMiddle;

		double checksum2 = (corner * corners) + (side * sides) + (middle * middles);
	}

	void iterate()
	{
		//new corner:
		//e.g. 1
		//squares that can transition: 2,6 = sideToCorner / corners
		//probability of being on 2,6: 2 * side
		// (sideToCorner / corners) * side
		double newSideToCorner1 = sideToCorner * side * 1/3;
		double newSideToCorner2 = sideToCorner * side * 1/2;
		double newCornerStays1 = cornerStays * corner * 1 / 3;
		double newCornerStays2 = cornerStays * corner * 1 / 2;
		double newCorner1 = newSideToCorner1 + newCornerStays1;
		double newCorner2 = newSideToCorner2 + newCornerStays2;
		double newCorner = (newCorner1 + newCorner2) * 0.5 / corners;

		double newCornerToSide1 = (cornerToSide * corner * 1 / 3);
		double newCornerToSide2 = (cornerToSide * corner * 1 / 4);
		double newSideToSide1 = (sideToSide * side * 1 / 3);
		double newSideToSide2 = (sideToSide * side * 1 / 6);
		double newMiddleToSide1 = (middleToSide * middle * 1 / 5);
		double newMiddleToSide2 = (middleToSide * middle * 1 / 8);
		double newSideStays1 = (sideStays * side * 1 / 4);
		double newSideStays2 = (sideStays * side * 1 / 2);
		double newSide1 = newCornerToSide1 + newSideToSide1 + newMiddleToSide1 + newSideStays1;
		double newSide2 = newCornerToSide2 + newSideToSide2 + newMiddleToSide2 + newSideStays2;
		double newSide = (newSide1 + newSide2) * 0.5 / sides;

		double newSideToMiddle1 = sideToMiddle * middle * 1 / 4;
		double newSideToMiddle2 = sideToMiddle * middle * 1 / 6;
		double newMiddleToMiddle1 = middleToMiddle * middle * 1 / 5;
		double newMiddleToMiddle2 = middleToMiddle * middle * 1 / 8;
		double newMiddleStays1 = middleStays * middle * 1 / 5;
		double newMiddleStays2 = middleStays * middle * 1 / 2;
		
		double newMiddle1 = newSideToMiddle1 + newMiddleToMiddle1 + newMiddleStays1;
		double newMiddle2 = newSideToMiddle2 + newMiddleToMiddle2 + newMiddleStays2;
		double newMiddle = (newMiddle1 + newMiddle2) * 0.5 / middles;

		corner = newCorner;
		side = newSide;
		middle = newMiddle;

		double checksum = (corner * corners) + (side * sides) + (middle * middles);

	}

};

int main()
{
	square s;
	s.iterate();

    return 0;
}


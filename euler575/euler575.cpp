// euler575.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"

#define SIZE 5
#define COUNT (SIZE*SIZE)
#define ONE ((double)1)

using namespace std;


class tile_type
{
public:
	double in, out;
	tile_type() : in(0), out(0) {}
};

class tile
{
public:
	int x, y, i;
	tile_type *tt_heads, *tt_tails;
	vector<tile*> neighbours;

	tile(int _i, 
		tile_type* tt_corner_heads, tile_type* tt_side_heads, tile_type* tt_middle_heads, 
		tile_type* tt_corner_tails, tile_type* tt_side_tails, tile_type* tt_middle_tails) : i(_i)
	{
		x = _i % SIZE;
		y = _i / SIZE;
		bool isLeftOrRight = x == 0 || x == (SIZE - 1);
		bool isTopOrBottom = y == 0 || y == (SIZE - 1);
		bool isSide = isLeftOrRight != isTopOrBottom;
		bool isCorner = isLeftOrRight && isTopOrBottom;

		if (isCorner)
		{
			tt_heads = tt_corner_heads;
			tt_tails = tt_corner_tails;
		}
		else if (isSide)
		{
			tt_heads = tt_corner_heads;
			tt_tails = tt_corner_tails;
		}
		else
		{
			tt_heads = tt_corner_heads;
			tt_tails = tt_corner_tails;
		}
	}
	

};


int main()
{
	vector<tile> tiles;
	//make the tiles
	for (long i = 0; i < COUNT; i++)
	{
		tile_type corner_h, side_h, middle_h, corner_t, side_t, middle_t;
		tile tile(i, &corner_h, &side_h, &middle_h, &corner_t, &side_t, &middle_t);
		tiles.push_back(tile);
	}
	long xdiff[] = { 0, 1, 0,-1 }; //T,R,B,L
	long ydiff[] = { 1, 0,-1, 0 };

	for (long i = 0; i < COUNT; i++)
	{
		tile& tile = tiles.at(i);
		for (long move = 0; i < 4; i++)
		{
			long newx = tile.x + xdiff[move];
			long newy = tile.y + ydiff[move];
			if (newx >= 0 && newx < SIZE && newy >= 0 && newy < SIZE)
			{
				long newi = (newy * SIZE) + newx;
				tile.neighbours.push_back(&tiles.at(newi));
			}
		}
	}

	//print neighbours map:
	

    return 0;
}


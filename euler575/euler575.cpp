// euler575.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"

#define SIZE 3
#define COUNT (SIZE*SIZE)
#define ONE ((double)1)

const double corners = 4;
const double sides = ((SIZE - 2) * 4);
const double middles = COUNT - sides - corners;

using namespace std;

enum tiletype
{
	corner,middle,side
};

class tile_type
{
public:
	stack<double> transfers;

	double gives, current;
	double weight;
	tiletype type;
	tile_type(long _weight, double _gives, tiletype _type) : gives(_gives), weight(_weight), current((double)_weight / COUNT), type(_type) {}
	vector<tile_type*> neighbours;

};


class tile
{
public:
	int x, y, i;
	tile_type *tt_heads, *tt_tails;

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
			tt_heads = tt_side_heads;
			tt_tails = tt_side_tails;
		}
		else
		{
			tt_heads = tt_middle_heads;
			tt_tails = tt_middle_tails;
		}
	}
	
	void add_neighbour(tile* neighbour)
	{
		tt_heads->neighbours.push_back(neighbour->tt_heads);
		tt_tails->neighbours.push_back(neighbour->tt_tails);
	}

};


int main()
{
	vector<tile> tiles;
	//make the tiles
	tile_type	corner_h(corners, ONE/3, corner), side_h(sides, ONE/4, side), middle_h(middles, ONE/5, middle), 
				corner_t(corners, ONE/4, corner), side_t(sides, ONE/6, side), middle_t(middles, ONE/8, middle);

	tile_type* tile_types[] = { &corner_t, &side_t, &middle_t };

	for (long i = 0; i < COUNT; i++)
	{
		tile tile(i, &corner_h, &side_h, &middle_h, &corner_t, &side_t, &middle_t);
		tiles.push_back(tile);
	}
	long xdiff[] = { 0, 1, 0,-1 }; //T,R,B,L
	long ydiff[] = { -1, 0, 1, 0 };

	for (long i = 0; i < COUNT; i++)
	{
		tile& t = tiles.at(i);
		for (long move = 0; move < 4; move++)
		{
			long newx = t.x + xdiff[move];
			long newy = t.y + ydiff[move];
			if (newx >= 0 && newx < SIZE && newy >= 0 && newy < SIZE)
			{
				long newi = (newy * SIZE) + newx;
				t.add_neighbour(&tiles.at(newi));
			}
		}

	}

	for (int i = 0; i < 10; i++)
	{
		map<pair<tiletype, tiletype>, double> transfers;

		for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
		{
			for (vector<tile_type*>::iterator it = tile_types[tt]->neighbours.begin(); it != tile_types[tt]->neighbours.end(); it++)
			{
				double amountToGive = (*it)->gives * (*it)->current;
				tile_types[tt]->transfers.push(-amountToGive) ;
				(*it)->transfers.push(amountToGive );
			}
		}
		for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
		{
			while (!tile_types[tt]->transfers.empty())
			{
				tile_types[tt]->current -= tile_types[tt]->transfers.top();
				tile_types[tt]->transfers.pop();
			}
			if (tt > 0) cout << ",";
			cout << fixed << setprecision(12) << tile_types[tt]->current;
		}
		cout << endl;
	}
	
	

    return 0;
}


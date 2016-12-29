// euler575.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"

#define SIZE 1000
#define COUNT (SIZE*SIZE)
#define ONE ((double)1)

const double corners = 4;
const double sides = ((SIZE - 2) * 4);
const double middles = COUNT - sides - corners;

using namespace std;

enum tiletype
{
	corner, middle, side
};

class tile_type
{
public:
	vector<double> transfers;

	double gives, current;
	double weight;
	tiletype type;
	tile_type(long _weight, double _gives, tiletype _type) : gives(_gives), weight(_weight), current((double)_weight / COUNT), type(_type), squares(0) {}
	vector<tile_type*> neighbours;
	double squares;
	
	double p_square() const
	{
		if (weight == 0) return 0;
		double result = current * (squares) / weight;
		return result;
	}
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

void adjust(tile_type* tt1, tile_type* tt2, tile_type* tt3)
{
	double checksum = tt1->current + tt2->current + tt3->current;
	double adjust = 1 / checksum;
	tt1->current *= adjust;
	tt2->current *= adjust;
	tt3->current *= adjust;
}


//wrong: 0.000989640562 / 0.000989640563
// ?  0.000989640557 / 0.000989640558 ?
int main()
{
	cout << fixed << setprecision(14);
	vector<tile> tiles;
	//make the tiles
	tile_type	corner_h(corners, ONE / 3, corner), side_h(sides, ONE / 4, side), middle_h(middles, ONE / 5, middle),
		corner_t(corners, ONE / 4, corner), side_t(sides, ONE / 6, side), middle_t(middles, ONE / 8, middle);

	tile_type* tile_types[] = { &corner_t, &side_t, &middle_t, &corner_h, &side_h, &middle_h };

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

	long square;
	for (long i = 1; (square = (i*i)) <= COUNT; i++)
	{
		tiles.at(square - 1).tt_heads->squares++;
		tiles.at(square - 1).tt_tails->squares++;
	}

	for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
	{
		for (vector<tile_type*>::iterator it = tile_types[tt]->neighbours.begin(); it != tile_types[tt]->neighbours.end(); it++)
		{
			double amountToGive = (*it)->gives;
			tile_types[tt]->transfers.push_back(-amountToGive);
			(*it)->transfers.push_back(amountToGive);
		}
	}


	unsigned long long i = 0;
	while(true)
	{
		i++;
		stack<pair<tile_type*, double>> gains;
		stack<pair<tile_type*, double>> losses;

		for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
		{
			double gainByThisTileType = 0;
			for (vector<tile_type*>::iterator it = tile_types[tt]->neighbours.begin(); it != tile_types[tt]->neighbours.end(); it++)
			{
				double gainFromThisNeighbour = (*it)->gives * (*it)->current / (*it)->weight;
				losses.push(pair<tile_type*, double>(*it, gainFromThisNeighbour));
				gainByThisTileType += gainFromThisNeighbour;
			}
			gains.push(pair<tile_type*, double>(tile_types[tt], gainByThisTileType));
		}
		while (!gains.empty())
		{
			gains.top().first->current += gains.top().second;
			gains.pop();
		}
		while (!losses.empty())
		{
			losses.top().first->current -= losses.top().second;
			losses.pop();
		}

		adjust(&corner_h, &side_h, &middle_h);
		adjust(&corner_t, &side_t, &middle_t);
		if (i % 10 == 0)
		{
			double checksum = 0;
			
			for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
			{
				checksum += tile_types[tt]->current;
			}
			double p_square_h = corner_h.p_square() + side_h.p_square() + middle_h.p_square();
			double p_square_t = corner_t.p_square() + side_t.p_square() + middle_t.p_square();
			double p_square = (p_square_h + p_square_t) / 2;

			cout << "checksum = " << checksum << ", p_square = " <<  p_square << endl;
		}
	}



	return 0;
}


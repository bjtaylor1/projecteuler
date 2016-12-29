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
		//probability of it being on a square is simply the probability of it being on this
		//tile type at all, multiplied by the proportion of this type's tiles that are squares (squares/weight).
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


//https://projecteuler.net/problem=575
//wrong: 0.000989640562 / 0.000989640563
// ?  0.000989640557 / 0.000989640558 ?
// 0.000989640561(28) = correct
int main()
{
	cout << fixed << setprecision(14);
	vector<tile> tiles;
	//make a tile type for each distinct type of tile, assigning it the probability that it transfers each time
	//there are 6 in total - 3 for the 'heads' case, 3 for the 'tails' case.
	//we treat the heads and tails cases completely separately right until the calculation of the result, since that's how they're described in the problem

	tile_type	
		corner_h(corners, ONE / 3, corner), side_h(sides, ONE / 4, side), middle_h(middles, ONE / 5, middle),
		corner_t(corners, ONE / 4, corner), side_t(sides, ONE / 6, side), middle_t(middles, ONE / 8, middle);

	tile_type* tile_types[] = { &corner_t, &side_t, &middle_t, &corner_h, &side_h, &middle_h };

	//assign each tile in the matrix a tile type
	for (long i = 0; i < COUNT; i++)
	{
		tile tile(i, &corner_h, &side_h, &middle_h, &corner_t, &side_t, &middle_t);
		tiles.push_back(tile);
	}


	long xdiff[] = { 0, 1, 0,-1 }; //T,R,B,L
	long ydiff[] = { -1, 0, 1, 0 };

	//work out each tile's neighbours:
	for (long i = 0; i < COUNT; i++)
	{
		tile& t = tiles.at(i);
		for (long move = 0; move < 4; move++) //look a the top, right, bottom and left tiles
		{
			long newx = t.x + xdiff[move];
			long newy = t.y + ydiff[move];

			//is it within the grid?
			if (newx >= 0 && newx < SIZE && newy >= 0 && newy < SIZE)
			{
				long newi = (newy * SIZE) + newx;
				t.add_neighbour(&tiles.at(newi));
			}
		}
	}

	//make a record of how many tiles of square numbers are in each tile type
	long square;
	for (long i = 1; (square = (i*i)) <= COUNT; i++)
	{
		tiles.at(square - 1).tt_heads->squares++;
		tiles.at(square - 1).tt_tails->squares++;
	}

	//work out how much probability each tile type transfers to its neighbours each time
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
	while(true) //simply stop the program when the number of decimal places > 12 has stopped changing (asymptotic)
	{
		i++;
		stack<pair<tile_type*, double>> gains;
		stack<pair<tile_type*, double>> losses;

		//for each tile type...
		for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
		{
			double gainByThisTileType = 0;
			//look at all the neighbours of this tile type
			//(this could probably be optimized by grouping the neighbouring tile types and summing up the TOTAL transfer for each one - 
			//some of the neighbours will be the same type)
			for (vector<tile_type*>::iterator it = tile_types[tt]->neighbours.begin(); it != tile_types[tt]->neighbours.end(); it++)
			{
				//the amount of probability in each tile is the CURRENT amount of probability in this tile type, divided by the number of tiles
				//in this tile type (the WEIGHT).
				double amountPerTile = (*it)->current / (*it)->weight;

				//the amount that this tile transfers is the amount in the tile, multiplied by the amount of probability it transfers
				double gainFromThisNeighbour = (*it)->gives * amountPerTile;

				//this particular neighbouring tile type loses this amount of probability
				losses.push(pair<tile_type*, double>(*it, gainFromThisNeighbour));

				//...whilst this tile type gains it
				gainByThisTileType += gainFromThisNeighbour;
			}
			//record the total gains for the tile type from all its neighbours
			gains.push(pair<tile_type*, double>(tile_types[tt], gainByThisTileType));
		}

		//amortize the gains and losses 
		//(don't do this during the loop, as the effect needs to be based only on the previous state, not the constantly-changing state,
		//i.e. each iteration is immutable)
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

		//correct rounding errors:
		adjust(&corner_h, &side_h, &middle_h);
		adjust(&corner_t, &side_t, &middle_t);

		//every so often, print the results
		if (i % 10 == 0)
		{
			double checksum = 0;
			
			for (int tt = 0; tt < sizeof(tile_types) / sizeof(tile_type*); tt++)
			{
				checksum += tile_types[tt]->current;
			}
			//work out the probability of it being on a square
			double p_square_h = corner_h.p_square() + side_h.p_square() + middle_h.p_square();
			double p_square_t = corner_t.p_square() + side_t.p_square() + middle_t.p_square();
			double p_square = (p_square_h + p_square_t) / 2;

			cout << "checksum = " << checksum << ", p_square = " <<  p_square << endl;
		}
	}
	return 0;
}


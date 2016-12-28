// euler575.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"

#define SIZE 5
#define COUNT (SIZE*SIZE)
#define ONE ((double)1)

using namespace std;

double strategy_heads_stay(long numMoves)
{
	return ONE / (numMoves + 1);
}

double strategy_heads_move(long numMoves)
{
	return ONE / (numMoves + 1);
}

double strategy_tails_stay(long numMoves)
{
	return 0.5;
}

double strategy_tails_move(long numMoves)
{
	return 0.5 / numMoves;
}

class square
{
public:
	static long nextid;
	static long created;
	static long destroyed;
	long id = nextid++;
	static long* numMoves;
	static long* isSide;
	static long* isCorner;
	static long* x;
	static long* y;
	static long get_i(const long x, const long y)
	{
		long res = (y * SIZE) + x;
		return res;
	}

	static void init()
	{
		numMoves = new long[COUNT];
		x = new long[COUNT];
		y = new long[COUNT];
		isSide = new long[COUNT];
		isCorner = new long[COUNT];
		for (long i = 0; i < COUNT; i++)
		{
			x[i] = i % SIZE;
			y[i] = i / SIZE;
			bool isLeftOrRight = x[i] == 0 || x[i] == (SIZE - 1);
			bool isTopOrBottom = y[i] == 0 || y[i] == (SIZE - 1);
			isSide[i] = isLeftOrRight || isTopOrBottom;
			isCorner[i] = isLeftOrRight && isTopOrBottom;
			if (isCorner[i]) numMoves[i] = 2;
			else if (isSide[i]) numMoves[i] = 3;
			else numMoves[i] = 4;
		}
	}

	static void de_init()
	{
		delete[] numMoves;
		delete[] isSide;
		delete[] isCorner;
		delete[] x;
		delete[] y;
	}

	double probability[COUNT];

	double p_square() const
	{
		int s;
		double result = 0;
		for (int squarenum = 1; (s = squarenum*squarenum) <= COUNT; squarenum++)
		{
			result += probability[s - 1];
		}
		return result;
	}

	void distribute(double& p_corner, double& p_side, double& p_middle) const
	{
		for (int i = 0; i < COUNT; i++)
		{
			if (isCorner[i]) p_corner += probability[i];
			else if (isSide[i]) p_side += probability[i];
			else p_middle += probability[i];
		}
	}

	void initialize()
	{
		for (long i = 0; i < COUNT; i++)
		{
			probability[i] = ONE / COUNT;
		}
	}

	square()
	{
		created++;
	}
	~square()
	{
		destroyed++;
	}

	square (const square* old, double (*get_p_move)(long), double (*get_p_stay)(long)) : square()
	{
		for (long i = 0; i < COUNT; i++)
		{
			double p_stay = get_p_stay(numMoves[i]);
			double p = old->probability[i] * p_stay;
			
			{
				//left:
				long moveX = x[i] - 1;
				if (moveX >= 0)
				{
					long move_i = get_i(moveX, y[i]);
					double pMove = get_p_move(numMoves[move_i]);
					p += old->probability[move_i] * pMove;;
				}
			}
			{
				//right:
				long moveX = x[i] + 1;
				if (moveX < SIZE)
				{
					long move_i = get_i(moveX, y[i]);
					double pMove = get_p_move(numMoves[move_i]);
					p += old->probability[move_i] * pMove;
				}
			}
			{
				//up:
				long moveY = y[i] - 1;
				if (moveY >= 0)
				{
					long move_i = get_i(x[i], moveY);
					double pMove = get_p_move(numMoves[move_i]);
					p += old->probability[move_i] * pMove;
				}
			}
			{
				//down:
				long moveY = y[i] + 1;
				if (moveY < SIZE)
				{
					long move_i = get_i(x[i], moveY);
					double pMove = get_p_move(numMoves[move_i]);
					p += old->probability[move_i] * pMove;
				}
			}
			
			probability[i] = p;
		}
	}

	double get_checksum() const
	{
		double checksum = 0;
		for (long i = 0; i < COUNT; i++)
		{
			checksum += probability[i];
		}
		return checksum;
	}
};

long square::nextid = 0;
long square::created = 0;
long square::destroyed = 0;
long* square::numMoves;
long* square::isSide;
long* square::isCorner;
long* square::x;
long* square::y;

int main()
{
	square::init();

	square* if_heads = new square;
	square* if_tails = new square;
	if_heads->initialize();
	if_tails->initialize();

	time_t start;
	time(&start);
	int i = 0;
	double totalElapsed = 0;
	while (true)
	{
		square* new_if_heads = new square(if_heads, &strategy_heads_move, &strategy_heads_stay);
		square* new_if_tails = new square(if_tails, &strategy_tails_move, &strategy_tails_stay);

		if ((i++ % 100) == 0)
		{
			time_t now;
			time(&now);
			double elapsed = difftime(now, start);
			if (elapsed > 2)
			{
				/*
				double p_square = (new_if_heads->p_square() + new_if_tails->p_square()) / 2;
				cout << fixed << setprecision(12) << p_square << ", i = " << i << endl;
				*/
				double p_corner = 0, p_side = 0, p_middle = 0;
				new_if_heads->distribute(p_corner, p_side, p_middle);
				cout << fixed << setprecision(12) << p_corner << ","
					<< fixed << setprecision(12) << p_side << ","
					<< fixed << setprecision(12) << p_middle << endl;
				totalElapsed += elapsed;
				start = now;
			}
		}

		delete if_heads;
		delete if_tails;
		if_heads = new_if_heads;
		if_tails = new_if_tails;
	}

	delete if_heads;
	delete if_tails;
	square::de_init();
	cout << "Created: " << square::created << ", destroyed = " << square::destroyed << " (" << (square::created - square::destroyed) << " diff)" << endl;

    return 0;
}


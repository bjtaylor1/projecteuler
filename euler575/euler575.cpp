// euler575.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"

#define SIZE 3
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
	static long* x;
	static long* y;
	static long* isSide;
	static long* isCorner;
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
			if (isCorner) numMoves[i] = 2;
			else if (isSide) numMoves[i] = 3;
			else numMoves[i] = 4;
		}
	}

	static void de_init()
	{
		delete[] numMoves;
		delete[] x;
		delete[] y;
	}

	double probability[COUNT];
	double p_corner, p_middle, p_side;

	double p_square() const
	{
		int s;
		double result = 0;
		for (int squarenum = 1; (s = squarenum*squarenum) <= COUNT; squarenum++)
		{
			if (isCorner[s]) result += p_corner;
			else if (isSide[s]) result += p_side;
			else result += p_middle;
		}
		return result;
	}

	void initialize()
	{
		for (int i = 0; i < COUNT; i++)
		{
			probability[i] = ONE / COUNT;
		}
		p_corner = ((double)4) / COUNT;
		p_side = ((double)(SIZE - 2)) / COUNT;
		p_middle = (COUNT - 4 * (SIZE - 1)) / COUNT;
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
long* square::x;
long* square::y;
long* square::isSide;
long* square::isCorner;

ostream& operator<<(ostream& os, const square& s)
{
	for (int y = 0; y < SIZE; y++)
	{
		for (int x = 0; x < SIZE; x++)
		{
			if (x > 0) cout << ",";
			cout << fixed << setprecision(12) << s.probability[square::get_i(x, y)];
		}
		cout << endl;
	}
	return os;
}

int main()
{
	square::init();

	square* if_heads = new square;
	square* if_tails = new square;
	if_heads->initialize();
	if_tails->initialize();

	cout << *if_heads << endl << endl << *if_tails << endl;


	time_t start;
	time(&start);
	int i = 0;
	double totalElapsed = 0;
	while (totalElapsed < 20)
	{
		square* new_if_heads = new square(if_heads, &strategy_heads_move, &strategy_heads_stay);
		square* new_if_tails = new square(if_tails, &strategy_tails_move, &strategy_tails_stay);

		if ((i++ % 1000) == 0)
		{
			time_t now;
			time(&now);
			double elapsed = difftime(now, start);
			if (elapsed > 2)
			{
				cout << "i = " << i << endl;
				cout << *if_heads << endl << endl << *if_tails << endl;
				totalElapsed += elapsed;
				start = now;
			}
		}

		delete if_heads;
		delete if_tails;
		if_heads = new_if_heads;
		if_tails = new_if_tails;
	}

	cout << *if_heads << endl << endl << *if_tails << endl;

	delete if_heads;
	delete if_tails;
	square::de_init();
	cout << "Created: " << square::created << ", destroyed = " << square::destroyed << " (" << (square::created - square::destroyed) << " diff)" << endl;

    return 0;
}


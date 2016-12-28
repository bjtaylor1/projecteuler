// euler575.cpp : Defines the entry polong for the console application.
//

#include "stdafx.h"

#define SIZE 5
#define COUNT (SIZE*SIZE)
#define ONE ((double)1)
#define TWO ((double)2)
#define THREE ((double)3)
#define FOUR ((double)4)
#define FIVE ((double)5)
#define NUMCORNERS ((double)4)
#define NUMSIDES (double)(SIZE - 2) * (double)4
#define NUMMIDDLES (double)(COUNT - ((SIZE - 1) * 4))

using namespace std;

class strategy
{
public:
	virtual double get_stay_corner() const = 0;
	virtual double get_move_corner() const = 0;
	virtual double get_stay_side() const = 0;
	virtual double get_move_side() const = 0;
	virtual double get_stay_middle() const = 0;
	virtual double get_move_middle() const = 0;
};

class strategy_heads
{
public:
	double get_stay_corner() const	{return ONE / 3; 	}
	double get_move_corner() const { return TWO / 3;  }
	
	double get_stay_side() const { return ONE / 4; }
	double get_move_side() const { return THREE / 4; }

	double get_stay_middle() const {return ONE / 5;	}
	double get_move_middle() const { return FOUR / 5; }
};

class strategy_tails
{
public:
	double get_stay_corner() const	{return ONE/TWO; 	}
	double get_move_corner() const { return ONE / TWO;  }
	
	double get_stay_side() const { return ONE / TWO; }
	double get_move_side() const { return ONE / TWO; }

	double get_stay_middle() const {return ONE / TWO;	}
	double get_move_middle() const { return ONE / TWO; }
};

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

	//double probability[COUNT];
	double p_corner, p_side, p_middle;

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
		p_corner = NUMCORNERS / COUNT;
		p_side = NUMSIDES / COUNT;
		p_middle = NUMMIDDLES / COUNT;
	}

	square()
	{
		created++;
	}
	~square()
	{
		destroyed++;
	}

	square (const square* old, const strategy* strat) : square()
	{
		p_corner = old->p_corner + ((strat->get_stay_corner() - strat->get_move_corner()) * ONE / NUMCORNERS);
		p_side = old->p_side + ((strat->get_stay_side() - strat->get_move_side()) * ONE / NUMSIDES);
		p_middle = old->p_middle + ((strat->get_stay_middle() - strat->get_move_middle()) * ONE / NUMMIDDLES);
	}

	double get_checksum() const
	{
		double checksum = p_side + p_middle + p_corner;
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
	strategy_heads strat_heads;
	strategy_tails strat_tails;
	strategy* pstrat_heads = (strategy*)&strat_heads;
	strategy* pstrat_tails = (strategy*)&strat_tails;

	if_heads->initialize();
	if_tails->initialize();

	time_t start;
	time(&start);
	int i = 0;
	double totalElapsed = 0;
	while (true)
	{
		square* new_if_heads = new square(if_heads, pstrat_heads);
		square* new_if_tails = new square(if_tails, pstrat_tails);

		if ((i++ % 100) == 0)
		{
			time_t now;
			time(&now);
			double elapsed = difftime(now, start);
			if (elapsed > 2)
			{
				double p_square = (new_if_heads->p_square() + new_if_tails->p_square()) / 2;
				cout << fixed << setprecision(12) << p_square << ", i = " << i << endl;

				totalElapsed += elapsed;
				start = now;
			}
		}

		delete if_heads;
		delete if_tails;
		if_heads = new_if_heads;
		if_tails = new_if_tails;
	}

	cout << "Heads, corner: " << if_heads->p_corner << " side: " << if_heads->p_side << " middle: " << if_heads->p_middle << endl;
	cout << "Tails, corner: " << if_tails->p_corner << " side: " << if_tails->p_side << " middle: " << if_tails->p_middle << endl;

	delete if_heads;
	delete if_tails;
	square::de_init();
	cout << "Created: " << square::created << ", destroyed = " << square::destroyed << " (" << (square::created - square::destroyed) << " diff)" << endl;

    return 0;
}


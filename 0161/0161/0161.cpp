#include "stdafx.h"

#define MAXINDSHAPE 3145728 /* 12 * 4 ^ 9 */
using namespace std;

/*
Solution: 20574308184277971
*/

int sizex, sizey;
map<mpz_class, long long> shapeSolutions;
map<mpz_class, mpz_class> shapeIndToCorrectShapeInd;
int board[9][12];


/* ******************** Prototypes ************************ */

long long int analyze(int x, int y, int nbTrio);
long long int analyzeMain(void);
void nextHole(int* x, int* y);
void show(void);
void cleanBoard(void);
mpz_class shapeIndex(void);


/* ******************** Main ************************ */
int main(void)
{
	long long int i, ns = 0;

	//for (i = 0; i < MAXINDSHAPE; i++) {
	//	shapeSolutions[i] = -1;
	//}

	sizex = 9;
	sizey = 12;

	ns = analyzeMain();
	printf("Number of solutions found in the %dx%d board: %lld\n", sizex, sizey, ns);

	return 0;
}

/* ******************** Implementations ************************ */



long long int analyze(int x, int y, int nbTrio)
{
	long long int nbS;

	nbS = 0;
	if (y >= sizey) {
		return 1;
	}

	auto shapeInd = shapeIndex(); 
	auto cached = shapeSolutions.find(shapeInd);

	if (cached != shapeSolutions.end())
	{
		return cached->second;
	}

	int nx, ny;

	/*       OX */
	/* Piece X  */
	if ((x < sizex - 1) && (y < sizey - 1)) {
		if (!board[x][y + 1] && !board[x + 1][y]) {
			nbTrio++;
			board[x][y] = board[x][y + 1] = board[x + 1][y] = nbTrio;
			nx = x;
			ny = y;
			nextHole(&nx, &ny);
			nbS += analyze(nx, ny, nbTrio);
			board[x][y] = board[x][y + 1] = board[x + 1][y] = 0;
			nbTrio--;
		}
	}

	/* Piece OXX */
	if (x < sizex - 2) {
		if (!board[x + 1][y] && !board[x + 2][y]) {
			nbTrio++;
			board[x][y] = board[x + 1][y] = board[x + 2][y] = nbTrio;
			nx = x;
			ny = y;
			nextHole(&nx, &ny);
			nbS += analyze(nx, ny, nbTrio);
			board[x][y] = board[x + 1][y] = board[x + 2][y] = 0;
			nbTrio--;
		}
	}

	/*       O */
	/* Piece X */
	/*       X */
	if (y < sizey - 2) {
		if (!board[x][y + 1] && !board[x][y + 2]) {
			nbTrio++;
			board[x][y] = board[x][y + 1] = board[x][y + 2] = nbTrio;
			nx = x;
			ny = y;
			nextHole(&nx, &ny);
			nbS += analyze(nx, ny, nbTrio);
			board[x][y] = board[x][y + 1] = board[x][y + 2] = 0;
			nbTrio--;
		}
	}

	/*       OX */
	/* Piece  X */
	if ((x < sizex - 1) && (y < sizey - 1)) {
		if (!board[x + 1][y] && !board[x + 1][y + 1]) {
			nbTrio++;
			board[x][y] = board[x + 1][y] = board[x + 1][y + 1] = nbTrio;
			nx = x;
			ny = y;
			nextHole(&nx, &ny);
			nbS += analyze(nx, ny, nbTrio);
			board[x][y] = board[x + 1][y] = board[x + 1][y + 1] = 0;
			nbTrio--;
		}
	}

	/*       O  */
	/* Piece XX */
	if ((x < sizex - 1) && (y < sizey - 1)) {
		if (!board[x][y + 1] && !board[x + 1][y + 1]) {
			nbTrio++;
			board[x][y] = board[x][y + 1] = board[x + 1][y + 1] = nbTrio;
			nx = x;
			ny = y;
			nextHole(&nx, &ny);
			nbS += analyze(nx, ny, nbTrio);
			board[x][y] = board[x][y + 1] = board[x + 1][y + 1] = 0;
			nbTrio--;
		}
	}

	/*        O */
	/* Piece XX */
	if ((x >= 1) && (y < sizey - 1)) {
		if (!board[x][y + 1] && !board[x - 1][y + 1]) {
			nbTrio++;
			board[x][y] = board[x][y + 1] = board[x - 1][y + 1] = nbTrio;
			nx = x;
			ny = y;
			nextHole(&nx, &ny);
			nbS += analyze(nx, ny, nbTrio);
			board[x][y] = board[x][y + 1] = board[x - 1][y + 1] = 0;
			nbTrio--;
		}
	}
	shapeSolutions[shapeInd] = nbS;
	return nbS;
}

long long int analyzeMain(void)
{
	int i, j;
	long long int n1, n2;

	cleanBoard();
	return analyze(0, 0, 0);
}


void nextHole(int* x, int* y)
{
	do {
		(*x)++;
		if (*x >= sizex) {
			*x = 0;
			(*y)++;
		}
	} while ((*y < sizey) && board[*x][*y]);
}

void show(void)
{
	int x, y;

	for (y = 0; y < sizey; y++) {
		for (x = 0; x < sizex; x++) {
			printf("%c", 'A' - 1 + board[x][y]);
		}
		printf("\n");
	}
	printf("\n");
}

void cleanBoard(void)
{
	int x, y;

	for (y = 0; y < sizey; y++) {
		for (x = 0; x < sizex; x++) {
			board[x][y] = 0;
		}
	}
}

mpz_class shapeIndex(void)
{
	mpz_class retval(0);
	long bit = 0;
	for (int x = 0; x < sizex; x++)
	{
		for (int y = 0; y < sizey; y++)
		{
			if (board[x][y]) mpz_setbit(retval.get_mpz_t(), bit);
			bit++;
		}
	}
	return retval; // .get_str();

}
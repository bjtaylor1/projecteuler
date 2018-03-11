// 0213bf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace std::chrono;
typedef unsigned long long ull;

const long NMAX = 30;
const long REPORT_DURATION = 5; //report every this many seconds
atomic<ull> total_empty;
atomic<ull> total_simulations;
string answer;
shared_mutex reportMutex;
time_point<system_clock> last_changed;
int flea_starts[NMAX * NMAX];
int square_starts[NMAX* NMAX];

int get_pos(int x, int y)
{
	return y * NMAX + x;
}

int get_y(int pos)
{
	return pos / NMAX;
}

int get_x(int pos)
{
	return pos % NMAX;
}

void report(ull empty, ull simulations)
{
	auto new_total_empty = total_empty.fetch_add(empty) + empty;
	auto new_total_simulations = total_simulations.fetch_add(simulations) + simulations;
	double result = ((double)new_total_empty) / ((double)new_total_simulations);
	stringstream ss;
	ss << fixed << setprecision(6) << result;
	string new_answer = ss.str();
	string current_answer;
	{
		shared_lock<shared_mutex> lock(reportMutex);
		current_answer.assign(answer);
	}
	if (current_answer != new_answer)
	{
		unique_lock<shared_mutex> lock(reportMutex);
		answer.assign(new_answer);
		last_changed = system_clock::now();

		struct tm timeinfo;
		time_t t = system_clock::to_time_t(last_changed);
		localtime_s(&timeinfo, &t);
		cout << "\rAnswer changed to " << new_answer << " at " << put_time(&timeinfo, "%d/%m/%y %H:%M:%S");
	}
}

void simulate()
{
	//run for ever
	while (true)
	{
		//report every 2 seconds
		for (auto start = system_clock::now();
			duration_cast<milliseconds>(system_clock::now() - start).count() < REPORT_DURATION * 1000;)
		{
			int fleas[NMAX * NMAX];
			int squares[NMAX * NMAX];
			memcpy(fleas, flea_starts, sizeof(int) * NMAX * NMAX);
			memset(squares, 0, sizeof(int)* NMAX * NMAX);
		}
	}
}

int main()
{
	for (int i = 0; i < NMAX * NMAX; i++)
	{
		auto x = get_x(i), y = get_y(i);
		auto pos = get_pos(x, y);
		cout << i << " == " << pos << "; " << x << "," << y << endl;
		flea_starts[i] = i;
		square_starts[i] = 1;
	}

	default_random_engine generator;
	uniform_int_distribution<int> edge(0, 2);
	for (int i = 0; i < 20; i++)
	{
		cout << edge(generator) << endl;
	}
    return 0;
}


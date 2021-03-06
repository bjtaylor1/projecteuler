// 0213bf.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"

using namespace std;
using namespace std::chrono;
typedef unsigned long long ull;

const long N = 30;
const long REPORT_DURATION = 30; //report every this many seconds
atomic<ull> total_empty;
atomic<ull> total_simulations;
string answer;
shared_mutex reportMutex;
time_point<system_clock> last_changed;
int flea_starts[N * N];
int square_starts[N* N];

int get_pos(int x, int y)
{
	return y * N + x;
}

int get_y(int pos)
{
	return pos / N;
}

int get_x(int pos)
{
	return pos % N;
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
		cout << "\rAnswer changed to " << new_answer << " at " << put_time(&timeinfo, "%d/%m/%y %H:%M:%S") << " by " << this_thread::get_id();
	}
}

void simulate()
{
	//run for ever
	default_random_engine generator;
	uniform_int_distribution<int> dists[]
	{
		uniform_int_distribution<int>(0,1),
		uniform_int_distribution<int>(0,2),
		uniform_int_distribution<int>(0,3)
	};
	
	while (true)
	{
		ull sub_total_empty = 0;
		ull sub_total_simulations = 0;
		//only report every few seconds
		for (auto start = system_clock::now();
			duration_cast<milliseconds>(system_clock::now() - start).count() < REPORT_DURATION * 1000;)
		{
			int empty = 0;
			int flea_positions[N * N];
			int squares[N * N];
			memcpy(flea_positions, flea_starts, sizeof(int) * N * N);
			memcpy(squares, square_starts, sizeof(int)* N * N);
			for (int belltoll = 0; belltoll < 50; belltoll++)
			{
				for (int flea = 0; flea < N; flea++)
				{
					int pos = flea_positions[flea];
					int x = get_x(pos), y = get_y(pos);
					vector<pair<int, int>> moves;
					if (x > 0) moves.push_back(pair<int,int>(x-1,y));
					if (x < N - 1) moves.push_back(pair<int, int>(x + 1, y));
					if (y > 0) moves.push_back(pair<int,int>(x, y-1));
					if (y < N - 1) moves.push_back(pair<int, int>(x, y + 1));;
					auto& dist = dists[moves.size() - 2];
					int move_id = dist(generator);
					auto& move = moves[move_id];
					int newx = move.first;
					int newy = move.second;
					int newpos = get_pos(newx, newy);
					flea_positions[flea] = newpos;
					auto oldpop = --squares[pos];
					if (oldpop == 0) empty++; //is now 0
					else if (oldpop < 0)
					{
						cout << "ERROR - population < 0" << endl;
						exit(1);
					}
					auto newpop = squares[newpos]++;
					if(newpop == 0)//was 0 - but is now populated
					{
						if (--empty < 0)
						{
							cout << "ERROR - empty < 0" << endl;
							exit(1);
						}
					}
				} //flea
			}//bell toll
			
			// end of 1 simulation
			sub_total_empty += empty;
			sub_total_simulations++;
		}
		report(sub_total_empty, sub_total_simulations);
	}
}

int main()
{
	for (int i = 0; i < N * N; i++)
	{
		auto x = get_x(i), y = get_y(i);
		auto pos = get_pos(x, y);
		flea_starts[i] = i;
		square_starts[i] = 1;
	}

	vector<thread> threads;
	for (int i = 0; i < 16; i++)
	{
		threads.push_back(thread(simulate));
	}
	for (vector<thread>::iterator it = threads.begin(); it != threads.end(); it++)
	{
		it->join();
	}

    return 0;
}


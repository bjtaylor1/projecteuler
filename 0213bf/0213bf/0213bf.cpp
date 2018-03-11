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

void report(ull empty, ull simulations)
{
	auto new_total_empty = total_empty.fetch_add(empty);
	auto new_total_simulations = total_simulations.fetch_add(simulations);
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

		}
	}
}

int main()
{
	last_changed = system_clock::now();
	struct tm timeinfo;
	time_t t = system_clock::to_time_t(last_changed);
	localtime_s(&timeinfo, &t);
	cout << put_time(&timeinfo, "%d/%m/%y %H:%M:%S") << endl;
	auto then = system_clock::now();
	auto now = system_clock::now();
	auto diff = duration_cast<milliseconds>(now - then);
	auto n = diff.count();
	pair<int, int> fleaStarts[900];
	for (int i = 0; i < NMAX; i++)
	{

	}
	default_random_engine generator;
	uniform_int_distribution<int> edge(0, 2);
	for (int i = 0; i < 20; i++)
	{
		cout << edge(generator) << endl;
	}
    return 0;
}


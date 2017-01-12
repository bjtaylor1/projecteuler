// threadtest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


using namespace std;

class queuefinished : exception {};

template<class T>
class blockingqueue
{
private:
	mutex m;
	queue<T> q;
	bool finished;
	int capacity;
public:
	blockingqueue(int Capacity) : capacity(Capacity), finished(false) {}
	void set_finished()
	{
		lock_guard<mutex> l(m);
		finished = true;
	}
	void push(T item)
	{
		while (true)
		{
			{
				lock_guard<mutex> lm(m);
				if (q.size() < capacity)
				{
					q.push(item);
					return;
				}
			}
			this_thread::sleep_for(chrono::milliseconds(10));
		}
	}

	T pop()
	{
		while (true)
		{
			{
				lock_guard<mutex> lm(m);
				if (finished) throw queuefinished();
				if (!q.empty())
				{
					T item = q.front();
					q.pop();
					return item;
				}
			}
			this_thread::sleep_for(chrono::milliseconds(10));
		}
	}
};

blockingqueue<int> q(6);

void process()
{
	try
	{
		while (true)
		{
			this_thread::sleep_for(0.5s);
			int i = q.pop();
			stringstream ss;
			ss << "Processing " << i << " on thread " << this_thread::get_id() << endl;
			cout << ss.str();
		}
	}
	catch (queuefinished)
	{
		stringstream ss;
		ss << "Thread " << this_thread::get_id() << " exiting - queue is finished." << endl;
		cout << ss.str();
	}
}

int main()
{
	vector<thread> threads;
	for (int i = 0; i < 4; i++) threads.push_back(thread(process));

	for (int i = 0; i < 30; i++)
	{
		q.push(i);
	}
	cout << "Pushed all values, setting finished." << endl;
	q.set_finished();
	for (vector<thread>::iterator thread = threads.begin(); thread != threads.end(); thread++)
	{
		thread->join();
	}
	cout << "All threads joined, exiting." << endl;
	return 0;
}


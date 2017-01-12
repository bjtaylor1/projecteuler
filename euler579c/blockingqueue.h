#pragma once
#ifndef BLOCKING_QUEUE_CLASS
#define BLOCKING_QUEUE_CLASS

using namespace std;

class queuefinished : public exception {};

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
			bool w = false;
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
				if (!q.empty())
				{
					T item = q.front();
					q.pop();
					return item;
				}
				else if (finished) throw queuefinished();
			}
			this_thread::sleep_for(chrono::milliseconds(10));
		}
	}
};

#endif

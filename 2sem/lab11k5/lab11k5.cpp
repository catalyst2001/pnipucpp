// lab11k5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <intrin.h>
#include <deque>
#include "../lab11k2/Time.h"
using namespace std;

template<typename T>
class Stack
{
	deque<T> dqueue;
public:
	Stack() {}
	~Stack() {}
	Stack(int size) { dqueue.resize(size); }

	Stack &operator=(const Stack &from) const {
		size_t sz = from.dqueue.size();
		dqueue.resize(sz);
		for (size_t i = 0; i < sz; i++)
			dqueue[i] = from.dqueue[i];

		return *this;
	}

	T &begin() const { return dqueue[0]; }
	T &end() const { return dqueue[dqueue.size() - 1]; }

	T &operator[](size_t idx) { return dqueue[idx]; }
	size_t operator()() const { return dqueue.size(); }
	size_t size() const { return dqueue.size(); }

	void insert_at(int idx, T elem) { dqueue.insert(dqueue.begin() + idx, elem); }
	void delete_at(int idx) { dqueue.erase(dqueue.begin() + idx); }

	void push(T elem) { dqueue.push_back(elem); }
	void push_back(T elem) { dqueue.push_back(elem); }
	void push_back_by_ref(T &elem) { dqueue.push_back(elem); }
};

class ParamClass
{
	Stack<Time> stack;
public:
	ParamClass() {}
	~ParamClass() {}
	
	void fill_rand()
	{
		for (int i = 0; i < 10; i++) {
			srand((unsigned int)__rdtsc());
			stack.push_back(Time(rand() % 60, rand() % 60));
		}
	}

	void print()
	{
		for (size_t i = 0; i < stack(); i++)
			cout << stack[i] << endl;
	}

	int compute_avg()
	{
		int avg = 0;
		for (size_t i = 0; i < stack(); i++)
			avg += stack[i].to_seconds();

		avg /= stack();
		return avg;
	}

	void delete_max()
	{
		int max = INT_MIN;
		int ind = 0;

		for (size_t i = 0; i < stack.size(); i++) {
			if (max < stack[i].to_seconds()) {
				max = stack[i].to_seconds();
				ind = i;
			}
		}
		stack.delete_at(ind);
	}

	void division_by_min()
	{
		int ind = 0;
		int min = INT_MAX;
		for (size_t i = 0; i < stack.size(); i++) {
			if (min > stack[i].to_seconds()) {
				min = stack[i].to_seconds();
				ind = i;
			}
		}

		//prevent division by zero
		if (!min)
			min = 1;

		for (size_t i = 0; i < stack.size(); i++)
			stack[i] = stack[i] / min;
	}

	size_t size() { return stack.size(); }

	//
	void compute_and_insert()
	{
		cout << "\ncompute avg and insert at K:\n";
		int k = rand() % stack.size();
		cout << "\navg:\nk = " << k << "\n";
		stack.insert_at(k, Time(compute_avg()));
		print();
	}
};

int main()
{
	ParamClass paramclass;
	paramclass.fill_rand();
	paramclass.print();

	cout << "\ncompute avg:\n";
	paramclass.compute_and_insert();
	
	cout << "\ndelete max from vector:\n";
	paramclass.delete_max();
	paramclass.print();

	cout << "\ndivision element by min:\n";
	paramclass.division_by_min();
	paramclass.print();
	return 0;
}
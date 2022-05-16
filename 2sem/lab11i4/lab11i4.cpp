// lab11i4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <deque>
#include <intrin.h>
#include "../lab3i/pair.h"
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

void print(Stack<Time> &s)
{
	for (size_t i = 0; i < s(); i++)
		cout << s[i] << endl;
}

void fill_rand(Stack<Time> &s)
{
	for (int i = 0; i < 10; i++) {
		srand((unsigned int)__rdtsc());
		s.push_back(Time(rand() % 60, rand() % 60));
	}
}

int compute_avg(Stack<Time> &s)
{
	int avg = 0;
	for (size_t i = 0; i < s(); i++)
		avg += s[i].to_seconds();

	avg /= s();
	return avg;
}

void delete_max(Stack<Time> &s)
{
	int max = INT_MIN;
	int ind = 0;

	for (size_t i = 0; i < s.size(); i++) {
		if (max < s[i].to_seconds()) {
			max = s[i].to_seconds();
			ind = i;
		}
	}
	s.delete_at(ind);
}

void division_by_min(Stack<Time> &vec)
{
	int ind = 0;
	int min = INT_MAX;
	for (size_t i = 0; i < vec.size(); i++) {
		if (min > vec[i].to_seconds()) {
			min = vec[i].to_seconds();
			ind = i;
		}
	}

	//prevent division by zero
	if (!min)
		min = 1;

	for (size_t i = 0; i < vec.size(); i++)
		vec[i] = vec[i] / min;
}

int main()
{
	Stack<Time> vec;
	fill_rand(vec);
	print(vec);

	int k = rand() % vec.size();
	cout << "\navg:\nk = " << k << "\n";
	vec.insert_at(k, Time(compute_avg(vec)));
	print(vec);

	cout << "\ndelete max from vector:\n";
	delete_max(vec);
	print(vec);

	cout << "\ndivision element by min:\n";
	division_by_min(vec);
	print(vec);
	return 0;
}
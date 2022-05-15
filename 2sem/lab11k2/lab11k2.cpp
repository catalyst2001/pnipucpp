// lab11k2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <vector>
#include <intrin.h>
#include "Time.h"
using namespace std;

void print(const vector<Time> &v)
{
	for (auto i : v)
		cout << i;
}

void fill_rand(vector<Time> &v)
{
	for (int i = 0; i < 10; i++) {
		srand((unsigned int)__rdtsc());
		v.push_back(Time(rand() % 60, rand() % 60));
	}
}

int compute_avg(vector<Time> &vec)
{
	int avg = 0;
	size_t i;
	for (i = 0; i < vec.size(); i++)
		avg += vec[i].to_seconds();

	avg /= vec.size();
	return avg;
}

void delete_max(vector<Time> &vec)
{
	int max = INT_MIN;
	int ind = 0;
	for (size_t i = 0; i < vec.size(); i++) {
		if (max < vec[i].to_seconds()) {
			max = vec[i].to_seconds();
			ind = i;
		}
	}
	vec.erase(vec.begin() + ind);
}

void division_by_min(vector<Time> &vec)
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
	vector<Time> vec;
	fill_rand(vec);
	print(vec);

	cout << "\navg:\n";
	cout << Time(compute_avg(vec));

	cout << "\ndelete max from vector:\n";
	delete_max(vec);

	cout << "\ndivision element by min:\n";
	division_by_min(vec);
	return 0;
}
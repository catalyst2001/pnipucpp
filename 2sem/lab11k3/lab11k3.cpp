// lab11k3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <vector>
#include <intrin.h> //получение счетчика tsc для задания seed
#include "../lab11k2/Time.h"
using namespace std;

class MyVector
{
	vector<Time> vec;
public:
	MyVector() {}
	~MyVector() {}
	MyVector(int size) { vec.reserve(size); }

	void print()
	{
		for (auto i : vec)
			cout << i;
	}

	void fill_rand()
	{
		for (int i = 0; i < 10; i++) {
			srand((unsigned int)__rdtsc());
			vec.push_back(Time(rand() % 60, rand() % 60));
		}
	}

	int compute_avg()
	{
		int avg = 0;
		size_t i;
		for (i = 0; i < vec.size(); i++)
			avg += vec[i].to_seconds();

		avg /= vec.size();
		return avg;
	}

	void delete_max()
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

	void division_by_min()
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
	int size() { return vec.size(); }
	void insert_at(Time elem, int idx) { vec.insert(vec.begin() + idx, elem); }
};

int main()
{
	MyVector vec(10);
	vec.fill_rand();
	vec.print();

	int k = rand() % vec.size();
	cout << "\navg:\nk = " << k << "\n";
	vec.insert_at(Time(vec.compute_avg()), k);
	vec.print();

	cout << "\ndelete max from vector:\n";
	vec.delete_max();
	vec.print();

	cout << "\ndivision element by min:\n";
	vec.division_by_min();
	vec.print();
	return 0;
}
// lab11i2.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <list>
#include <time.h>
#include <assert.h>
using namespace std;

void print(list<int> &lst)
{
	for (list<int>::iterator it = lst.begin(); it != lst.end(); it++)
		cout << (*it) << endl;
}

int &get_from_list(list<int> &lst, int idx)
{
	assert(idx >= 0 || idx < (int)lst.size());
	list<int>::iterator it = lst.begin();
	for (int i = 0; it != lst.end() && i < idx; i++)
		it++;

	return *it;
}

void delete_from_list(list<int> &lst, int idx)
{
	assert(idx >= 0 || idx < (int)lst.size());
	list<int>::iterator it = lst.begin();
	for (int i = 0; i < idx; i++)
		it++;

	lst.erase(it);
}

//-------

void fill_random(list<int> &lst, int size)
{
	lst.clear();
	for (size_t i = 0; i < size; i++)
		lst.push_back((rand() % 100) - 50);
}

int find_average(list<int> &lst)
{
	int average = 0;
	for (size_t i = 0; i < lst.size(); i++)
		average += get_from_list(lst, i);

	average /= lst.size();
	return average;
}

void add_avg_to_list(list<int> &lst)
{
	int k = rand() % lst.size();
	cout << "k=" << k << "\n";
	get_from_list(lst, k) = find_average(lst);
}

void del_max(list<int> &lst)
{
	int ind = 0;
	int max = INT_MIN;
	for (size_t i = 0; i < lst.size(); i++) {
		int val = get_from_list(lst, i);
		if (max < val) {
			max = val;
			ind = i;
		}
	}
	delete_from_list(lst, ind);
}

void div_by_min(list<int> &lst)
{
	size_t i;
	int min = INT_MAX;
	for (i = 0; i < lst.size(); i++) {
		int val = get_from_list(lst, i);
		if (val < min)
			min = val;
	}

	for (i = 0; i < lst.size(); i++)
		get_from_list(lst, i) /= min;
}

int main()
{
	srand((unsigned int)time(0));
	const int size = 10;
	list<int> lst(size);
	fill_random(lst, size);
	print(lst);

	cout << "\nadd_avg_to_list\n";
	add_avg_to_list(lst);
	print(lst);

	cout << "\ndel_max\n";
	del_max(lst);
	print(lst);

	cout << "\ndiv_by_min\n";
	div_by_min(lst);
	print(lst);
	return 0;
}
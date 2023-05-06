// lab11i1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
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
	assert(idx >= 0 || idx < lst.size());
	list<int>::iterator it = lst.begin();
	for (int i = 0; it != lst.end() && i < idx; i++)
		it++;

	return *it;
}

void delete_from_list(list<int> &lst, int idx)
{
	assert(idx >= 0 || idx < lst.size());
	list<int>::iterator it = lst.begin();
	for (int i = 0; i < idx; i++)
		it++;

	lst.erase(it);
}

int main()
{
	size_t i;
	list<int> list1;
	srand(time(NULL));
	for (i = 0; i < 10; i++)
		list1.push_back((rand() % 100));

	print(list1);

	// average
	cout << "\nfind avg:\n";
	int average = 0;
	for (i = 0; i < list1.size(); i++)
		average += get_from_list(list1, i);

	average /= list1.size();
	cout << "Avg: " << average << "\n";

	//delete maximum
	struct { int m, i; } maxd = { INT_MIN, 0 };
	for (i = 0; i < list1.size(); i++) {
		int val = get_from_list(list1, i);
		if (maxd.m < val) {
			maxd.m = val;
			maxd.i = i;
		}
	}

	//delete maximum
	cout << "\ndelete maximum:\n";
	delete_from_list(list1, maxd.i);
	print(list1);

	// find min
	int min = INT_MAX;
	for (i = 0; i < list1.size(); i++) {
		int val = get_from_list(list1, i);
		if (val < min)
			min = val;
	}
	

	//division by min
	cout << "\ndivision by min:\n";
	for (i = 0; i < list1.size(); i++)
		get_from_list(list1, i) /= min;

	print(list1);
	return 0;
}
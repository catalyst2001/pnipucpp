// lab11i3.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <list>
#include <time.h>
#include <assert.h>
#include "../lab3i/pair.h"

class pClass
{
	std::list<pair> lst;
public:
	pClass() {}
	pClass(int size) { lst.resize(size); }
	~pClass() {}

	void print()
	{
		for (std::list<pair>::iterator it = lst.begin(); it != lst.end(); it++)
			std::cout << (*it) << std::endl;
	}

	pair &get_from_list(int idx)
	{
		assert(idx >= 0 || idx < (int)lst.size());
		std::list<pair>::iterator it = lst.begin();
		for (int i = 0; it != lst.end() && i < idx; i++)
			it++;

		return *it;
	}

	void delete_from_list(int idx)
	{
		assert(idx >= 0 || idx < (int)lst.size());
		std::list<pair>::iterator it = lst.begin();
		for (int i = 0; i < idx; i++)
			it++;

		lst.erase(it);
	}

	//-------

	void fill_random(int size)
	{
		lst.clear();
		for (size_t i = 0; i < size; i++) {
			pair temp((rand() % 100) - 50, (double)((rand() % 100) - 50));
			lst.push_back(temp);
		}
	}

	pair find_average()
	{
		pair average;
		for (size_t i = 0; i < lst.size(); i++)
			average += get_from_list(i);

		average /= lst.size();
		return average;
	}

	void add_avg_to_list()
	{
		int k = rand() % lst.size();
		std::cout << "k=" << k << "\n";
		get_from_list(k) = find_average();
	}

	void del_max()
	{
		int ind = 0;
		pair max = pair(0, INT_MIN);
		for (size_t i = 0; i < lst.size(); i++) {
			pair val = get_from_list(i);
			if (max < val) {
				max = val;
				ind = i;
			}
		}
		delete_from_list(ind);
	}

	void div_by_min()
	{
		size_t i;
		pair min = pair(0, INT_MAX);
		for (i = 0; i < lst.size(); i++) {
			pair val = get_from_list(i);
			if (val < min)
				min = val;
		}

		for (i = 0; i < lst.size(); i++)
			get_from_list(i) /= min;
	}
};

int main()
{
	srand((unsigned int)time(0));
	const int size = 10;
	pClass pclass(size);
	pclass.fill_random(size);
	pclass.print();

	std::cout << "\nadd_avg_to_list\n";
	pclass.add_avg_to_list();
	pclass.print();

	std::cout << "\ndel_max\n";
	pclass.del_max();
	pclass.print();

	std::cout << "\ndiv_by_min\n";
	pclass.div_by_min();
	pclass.print();
	return 0;
}
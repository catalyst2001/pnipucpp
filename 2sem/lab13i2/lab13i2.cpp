#include <iostream>
#include <queue>
#include <vector>
#include <intrin.h>
#include "../lab3i/pair.h"
#include <algorithm>
#include <functional>
#include <numeric>

void print(std::vector<pair> &q)
{
	for (auto i : q)
		std::cout << i;
}

template <class T, class S, class C>
S& Container(std::priority_queue<T, S, C>& q) {
	struct HackedQueue : private std::priority_queue<T, S, C> {
		static S& Container(std::priority_queue<T, S, C>& q) {
			return q.*&HackedQueue::c;
		}
	};
	return HackedQueue::Container(q);
}

int main()
{
	srand((unsigned int)__rdtsc());
	std::priority_queue<pair> tasks;

	std::vector<pair> &tq = Container(tasks);
	for (size_t i = 0; i < 10; i++) {
		int val1 = rand() % 60;
		int val2 = rand() % 60;
		tq.insert(tq.begin() + 5, pair(val1, val2));
	}
	print(tq);

	//3 задание: найти среднее арифметическое и добавить его на заданную позицию
	auto val_average = std::accumulate(tq.begin(), tq.end(), 0, [](int accumulator, pair& a)
	{
		return accumulator + a.getValue();
	}) / tq.size();
	std::cout << std::endl << "val average is " << val_average << std::endl;
	std::cout << std::endl << "inserted data: " << std::endl;
	int k = 5; //позиция для вставки
	tq.insert(tq.begin() + k, pair(k, val_average));
	print(tq);

	//4 задание: найти элементы с ключами из диапазона и удалить из контейрера
	auto tq_new = std::remove_if(tq.begin(), tq.end(), [](pair& a)
	{
		return a > 30 && a < 60;
	});
	tq.erase(tq_new, tq.end());
	std::cout << std::endl << "remove_if\n";
	print(tq);

	//5 задание: из каждого элемента вычесть среднее арифметическое
	auto val_avg = std::accumulate(tq.begin(), tq.end(), 0, [](int accumulator, pair& a)
	{
		return accumulator + a.getValue();
	}) / tq.size();
	std::cout << std::endl << "val average is " << val_avg << std::endl;

	std::for_each(tq.begin(), tq.end(), [val_avg](pair& a) {
		a.setPair(a.getKey(), a.getValue() - val_avg);
	});
	print(tq);


	std::sort(tq.begin(), tq.end(), [](pair& a, pair& b) {
		return a > b;
	});
	std::cout << std::endl << "sorted:" << std::endl;
	print(tq);

	return 0;
}
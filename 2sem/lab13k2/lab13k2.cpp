﻿#include <iostream>
#include <queue>
#include <intrin.h>
#include "../lab12k2/myTime.h"
#include <algorithm>
#include <functional>
#include <numeric>

void print(std::queue<myTime> &q)
{
	for (auto i : q._Get_container())
		std::cout << i << std::endl;
}

int main()
{
	srand((unsigned int)__rdtsc());
	std::queue<myTime> tq;
	for (size_t i = 0; i < 10; i++) {
		int min = rand() % 60;
		int sec = rand() % 60;
		tq.push(myTime(min, sec));
	}
	print(tq);

	//1 задание найти макс элемент и добавить его в конец
	std::deque<myTime>::const_iterator max_time = std::max_element(tq._Get_container().begin(), tq._Get_container().end(), [](myTime& a, myTime& b)
	{
		return a < b;
	});
	std::cout << "Max: " << (*max_time).get_min() << ":" << (*max_time).get_sec() << "\n";

	tq.push(*max_time);
	print(tq);

	//2 задание найти элемент с заданным ключем (больше 30-ти минут) и удалить из контейрера
	auto tq_new = std::remove_if(tq._Get_container().begin(), tq._Get_container().end(), [](myTime& a)
	{
		return a.get_min() > 30;
	});
	tq._Get_container().erase(tq_new, tq._Get_container().end());
	std::cout << "remove_if\n";
	print(tq);

	//3 к каждому элементу добавить среднее арифметическое элементов контейнера
	auto sec_avg = std::accumulate(tq._Get_container().begin(), tq._Get_container().end(), 0, [](int accumulator, myTime& a)
	{
		return accumulator + ((a.get_min() * 60) + a.get_sec());
	}) / tq.size();
	int avg_min = (int)floor(sec_avg / 60);
	int avg_sec = (sec_avg - (avg_min * 60));
	std::cout << "time average is " << avg_min << ":" << avg_sec << std::endl;

	std::for_each(tq._Get_container().begin(), tq._Get_container().end(), [avg_min, avg_sec](myTime& a) {
		a.set_min(a.get_min() + avg_min);
		a.set_sec(a.get_sec() + avg_sec);
	});
	print(tq);


	std::sort(tq._Get_container().begin(), tq._Get_container().end(), [](myTime& a, myTime& b) {
		return a > b;
	});
	std::cout << "sorted:" << std::endl;
	print(tq);

	return 0;
}
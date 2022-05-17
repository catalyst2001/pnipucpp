// lab13k1.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <deque>
#include <intrin.h>
#include "../lab12k2/myTime.h"
#include <algorithm>

void print(std::deque<myTime> &q)
{
	for (auto i : q)
		std::cout << i << std::endl;
}

int main()
{
	srand((unsigned int)__rdtsc());
	std::deque<myTime> tq;
	for (size_t i = 0; i < 10; i++) {
		int min = rand() % 60;
		int sec = rand() % 60;
		tq.push_back(myTime(min, sec));
	}
	print(tq);

	auto find_max = [](std::deque<myTime> &tq, myTime max_init) -> myTime
	{
		myTime max = max_init;
		for (size_t i = 0; i < tq.size(); i++)
			if (max < tq[i])
				max = tq[i];

		return max;
	};

	std::cout << "Max out:\n";
	myTime max_time = find_max(tq, myTime(0, 0));
	std::cout << "Max: " << max_time.get_min() << ":" << max_time.get_sec() << "\n";

	myTime end_time = tq[tq.size() - 1];
	struct equal_filter {
		equal_filter() {}
		~equal_filter() {}
		equal_filter(myTime &t) : max(t) {}

		myTime max;
		bool operator()(myTime a) { return a == max; }
	};
	std::replace_if(tq.begin(), tq.end(), equal_filter(end_time), max_time);
	print(tq);


	return 0;
}
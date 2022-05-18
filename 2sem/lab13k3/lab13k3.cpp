#include <iostream>
#include <map>
#include <intrin.h>
#include "../lab12k2/myTime.h"
#include <algorithm>
#include <functional>
#include <numeric>

void print(std::multimap<int, myTime> &q)
{
	for (auto i : q)
		std::cout << i.second << std::endl;
}

template<typename AssociativeContainer, typename Predicate>
void erase_if(AssociativeContainer& container, Predicate shouldRemove)
{
	for (auto it = begin(container); it != end(container);)
	{
		if (shouldRemove(*it))
		{
			it = container.erase(it);
		}
		else
		{
			++it;
		}
	}
}

int main()
{
	srand((unsigned int)__rdtsc());
	std::multimap<int, myTime> tq;
	for (size_t i = 0; i < 10; i++) {
		int min = rand() % 60;
		int sec = rand() % 60;
		tq.insert(std::pair<int, myTime>(i, myTime(min, sec)));
	}
	print(tq);

	//1 задание найти макс элемент и добавить его в конец
	auto max_time = std::max_element(tq.begin(), tq.end(), [](std::pair<int, myTime> a, std::pair<int, myTime> b)
	{
		return a.second < b.second;
	});
	std::cout << "Max: " << (*max_time).second.get_min() << ":" << (*max_time).second.get_sec() << "\n";

	tq.insert(std::pair<int, myTime>((int)tq.size(), (*max_time).second));
	print(tq);
	
	//2 задание найти элемент с заданным ключем (больше 30-ти минут) и удалить из контейрера
	erase_if(tq, [](std::pair<int, myTime> a)
	{
		return a.second.get_min() > 30;
	});
	std::cout << "remove_if\n";
	print(tq);
	
	//3 к каждому элементу добавить среднее арифметическое элементов контейнера
	auto sec_avg = std::accumulate(tq.begin(), tq.end(), 0, [](int accumulator, std::pair<int, myTime> a)
	{
		return accumulator + ((a.second.get_min() * 60) + a.second.get_sec());
	}) / tq.size();
	int avg_min = (int)floor(sec_avg / 60);
	int avg_sec = (sec_avg - (avg_min * 60));
	std::cout << "time average is " << avg_min << ":" << avg_sec << std::endl;
	
	std::for_each(tq.begin(), tq.end(), [avg_min, avg_sec](std::pair<int, myTime> a) {
		a.second.set_min(a.second.get_min() + avg_min);
		a.second.set_sec(a.second.get_sec() + avg_sec);
	});
	print(tq);

	return 0;
}
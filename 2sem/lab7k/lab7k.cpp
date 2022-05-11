// lab7k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "vector4.h"

class Time
{
	int min, sec;
public:
	Time() : min(0), sec(0) {}
	Time(int m, int s) : min(m), sec(s) {}
	Time(const Time &t) {
		min = t.min;
		sec = t.sec;
	}

	Time operator*(const Time &t) { return Time(min * t.min, sec * t.sec); }
	Time operator*(const int t) { return Time(min * t, sec * t); }

	friend std::ostream &operator<<(std::ostream &out, const Time &tt)
	{
		std::cout << tt.min << " : " << tt.sec << "\n";
		return out;
	}

	friend std::istream &operator>>(std::istream &in, Time &tt)
	{
		std::cout << "enter min : sec:\n";
		std::cin >> tt.min >> tt.sec;
		return in;
	}
};

int main()
{
	vector4<int> vec1(5);
	std::cin >> vec1;
	std::cout << vec1;

	for (int i = 0; i < vec1(); i++)
		std::cout << vec1[i] << "\n";

	vector4<int> res = vec1 * 2;
	std::cout << res << "\n";

	Time time;
	std::cin >> time;
	std::cout << "time: " << time << "\n";

	return 0;
}
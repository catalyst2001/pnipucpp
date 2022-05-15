#pragma once
#include <iostream>
using namespace std;
class Time
{
	int min, sec;
public:
	Time() { min = 0; sec = 0; };
	Time(int m, int s) { min = m; sec = s; }
	Time(const Time&t) { min = t.min; sec = t.sec; }
	~Time() {};
	int get_min() { return min; }
	int get_sec() { return sec; }
	void set_min(int m) { min = m; }
	void set_sec(int s) { sec = s; }
	//перегруженные операции
	Time&operator=(const Time&);
	Time operator+(const Time&);
	Time operator/(const Time&);
	Time operator/(const int&);
	bool operator >(const Time&);
	bool operator <(const Time&);
	//глобальные функции ввода-вывода
	friend istream& operator>>(istream&in, Time&t);
	friend ostream& operator<<(ostream&out, const Time&t);
};

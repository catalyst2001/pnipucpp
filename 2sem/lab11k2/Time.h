#pragma once
#include <iostream>
using namespace std;

class Time
{
	int min, sec;
public:
	Time() : min(0), sec(0) {}
	Time(int seconds);
	Time(int m, int s) : min(m), sec(s) {}
	Time(const Time &t) : min(t.min), sec(t.sec) {}
	~Time() {}

	int get_min() { return min; }
	int get_sec() { return sec; }
	void set_min(int m) { min = m; }
	void set_sec(int s);
	int to_seconds();
	void print();

	bool operator==(const Time &t);
	bool operator!=(const Time &t);
	bool operator<(const Time &t);
	bool operator>(const Time &t);
	const Time& operator/(const Time &t);
	const Time& operator/(int i);
	const Time& operator+(const Time &t);
	friend std::istream &operator>>(std::istream &in, Time &t);
	friend std::ostream &operator<<(std::ostream &out, const Time &t);
};
#pragma once
#include <istream>
#include <iostream>

class myTime
{
	int min, sec;
public:
	myTime() : min(0), sec(0) {}
	myTime(int m, int s) : min(m), sec(s) {}
	myTime(const myTime &t) : min(t.min), sec(t.sec) {}
	~myTime() {}

	int get_min() { return min; }
	int get_sec() { return sec; }
	void set_min(int m) { min = m; }
	void set_sec(int s);
	void print();

	bool operator==(const myTime &t);
	bool operator!=(const myTime &t);
	bool operator<(const myTime &t);
	bool operator>(const myTime &t);
	const myTime& operator/(const myTime &t);
	const myTime& operator/(int i);
	const myTime& operator+(const myTime &t);
	friend std::istream &operator>>(std::istream &in, myTime &t);
	friend std::ostream &operator<<(std::ostream &out, const myTime &t);
};

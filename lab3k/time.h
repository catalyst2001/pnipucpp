#pragma once
#include <istream>
#include <iostream>

class time
{
	int min, sec;
public:
	time() : min(0), sec(0) {}
	time(int m, int s) : min(m), sec(s) {}
	time(const time &t) : min(t.min), sec(t.sec) {}
	~time() {}

	int get_min() { return min; }
	int get_sec() { return sec; }
	void set_min(int m) { min = m; }
	void set_sec(int s);
	void print();

	bool operator==(const time &t);
	bool operator!=(const time &t);
	friend std::istream &operator>>(std::istream &in, time &t);
	friend std::ostream &operator<<(std::ostream &out, const time &t);
};

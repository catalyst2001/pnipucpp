#pragma once
#include <iostream>

class pair2
{
	int first;
	double next;
public:
	pair2() : first(0), next(0.) {};
	pair2(int f, double n) : first(f), next(n) {};
	~pair2() {}

	pair2 operator+(const pair2 &p);

	friend std::istream &operator>>(std::istream &in, pair2 &p);
	friend std::ostream &operator<<(std::ostream &out, const pair2 &p);
};
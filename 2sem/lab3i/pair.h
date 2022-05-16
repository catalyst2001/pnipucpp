#pragma once
#include <iostream>

class pair
{
	int one;
	double two;
public:
	pair() { one = 0, two = 0.0; }
	pair(int o, double t) { one = 0, two = t; }

	void setPair(int o, double t) { one = o, two = t; }
	int getOne() { return one; }
	double getTwo() { return two; }

	bool operator>(const pair &p);
	bool operator>(const int &p);
	bool operator>(const double &p);
	bool operator<(const pair &p);
	bool operator<(const int &p);
	bool operator<(const double &p);

	pair &operator++(); //prefix
	pair operator++(int); //postfix
	pair operator+=(const pair &p);
	pair operator/=(const pair &p);
	pair operator/=(const int &p);

	friend std::istream &operator>>(std::istream &in, pair &p);
	friend std::ostream &operator<<(std::ostream &out, const pair &p);
};
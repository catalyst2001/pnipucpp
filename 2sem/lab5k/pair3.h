#pragma once
#include <iostream>
#include "IObject.h"

class pair3 : public IObject
{
protected:
	int first, second;
public:
	pair3() : first(0), second(0) {};
	pair3(int f, int s) : first(f), second(s) {};
	pair3(const pair3 &p) { *this = p; };
	~pair3() { };

	void set_first(int f) { first = f; }
	int get_first() { return first; }
	void set_second(int s) { second = s; }
	int get_second() { return second; }

	void show(); //реализация виртуальной функции

	pair3 operator+(const pair3 &p);
	friend std::istream &operator>>(std::istream &i, pair3 &p);
	friend std::ostream &operator<<(std::ostream &out, const pair3 &p);
};

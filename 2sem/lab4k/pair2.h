#include "../lab3i/pair.h"
#pragma once
#include <iostream>

class pair2
{
protected:
	int first, second;
public:
	pair2() : first(0), second(0) {
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	};
	pair2(int f, int s) : first(f), second(s) {
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	};
	pair2(const pair2 &p) {
		*this = p;
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	};
	~pair2() {
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	};

	void set_first(int f) { first = f; }
	int get_first() { return first; }
	void set_second(int s) { second = s; }
	int get_second() { return second; }
	
	pair2 operator+(const pair2 &p);

	friend std::istream &operator>>(std::istream &i, pair2 &p);
	friend std::ostream &operator<<(std::ostream &out, const pair2 &p);
};


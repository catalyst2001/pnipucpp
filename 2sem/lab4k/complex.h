#pragma once
#include "pair2.h"

class complex : public pair2
{
public:
	complex() {
		first = 0;
		second = 0;
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	}
	complex(int v, int i) {
		first = v;
		second = i;
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	}
	complex(const complex &c) {
		*this = c;
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	}
	~complex() {
		std::cout << __FUNCSIG__ " for object " << this << " called" << std::endl;
	}

	pair2 &get_pair() { return *this; }

	complex operator*(const complex &c);
	complex operator-(const complex &c);
	friend std::istream &operator>>(std::istream &i, complex &c);
	friend std::ostream &operator<<(std::ostream &out, const complex &c);
};


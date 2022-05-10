#pragma once
#include <iostream>
#include "IObject.h"

class complex2 : public IObject
{
	int real_part, imaginary_part;
public:
	complex2() : real_part(0), imaginary_part(0) {}
	complex2(int r, int i) : real_part(r), imaginary_part(i) {}
	complex2(const complex2 &c) { *this = c; }
	~complex2() {}

	void show();

	complex2 operator*(const complex2 &c);
	complex2 operator-(const complex2 &c);
	friend std::istream &operator>>(std::istream &i, complex2 &c);
	friend std::ostream &operator<<(std::ostream &out, const complex2 &c);
};


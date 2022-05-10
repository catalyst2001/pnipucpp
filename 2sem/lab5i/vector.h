#pragma once
#include <iostream>
#include "base.h"

class vector
{
	base **elems;
	int cur, size;
public:
	vector() : elems(nullptr), cur(0), size(0) {};
	vector(int size);
	~vector();
	void add(base *e);
	friend std::ostream& operator<<(std::ostream &out, const vector &vec);
};
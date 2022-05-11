#pragma once
#include <iostream>
#include <assert.h>

class iter
{
	friend class vector2;
	int *elem;
public:
	iter() : elem(nullptr) {}
	iter(const iter &it) { elem = it.elem; }

	bool operator==(const iter &it) { return elem == it.elem; }
	bool operator!=(const iter &it) { return elem != it.elem; }
	void operator++(int) { ++elem; }
	void operator--(int) { --elem; }

	iter operator-(int n);

	int &operator*() const { return *elem; }
};

class vector2
{
	int size, *p_data;
	iter beginit, endit;
public:
	vector2() : p_data(nullptr), size(0) {
		std::cout << __FUNCSIG__ " called for object " << this << "\n";
	}
	vector2(int s);
	vector2(int s, int v);
	~vector2();

	vector2 operator=(const vector2 &v);
	int &operator[](int idx);
	vector2 &operator*(const int i);
	int operator()();

	int Size() { return size; };

	int *begin() { return &p_data[0]; }
	int *end() { return &p_data[size]; }

	iter first() { return beginit; }
	iter last() { return endit; }
	friend std::ostream &operator<<(std::ostream &out, const vector2 &vec);
	friend std::istream &operator>>(std::istream &in, vector2 &vec);
};
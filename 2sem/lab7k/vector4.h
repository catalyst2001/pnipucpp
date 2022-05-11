#pragma once
#include <iostream>
#include <string.h>

#define assert(x) if(!(x)) __asm int 3h;

template<typename _T>
class vector4
{
	int size, curr;
	_T *p_data;

	void copy_from(const vector4<_T> &from) {
		// clear current vector heap if allocated
		if (p_data)
			delete[] p_data;

		curr = 0;
		size = from.size;
		p_data = new _T[size];
		memcpy(p_data, from.p_data, sizeof(_T) * from.size);
	}

public:
	vector4() : p_data(nullptr), size(0), curr(0) {
		std::cout << __FUNCSIG__ " called for object " << this << "\n";
	}

	vector4(int s) {
		//std::cout << __FUNCSIG__ " called for object " << this << "\n";
		size = s;
		assert(size > 0);
		p_data = new _T[size];
	}

	vector4(int s, int v) {
		//std::cout << __FUNCSIG__ " called for object " << this << "\n";
		size = s;
		assert(size > 0);

		p_data = new int[size];
		for (int i = 0; i < size; i++)
			p_data[i] = v;
	}

	vector4(const vector4<_T> &from) { copy_from(from); }

	~vector4() {
		//std::cout << __FUNCSIG__ " called for object " << this << "\n";
		if (p_data > 0) {
			delete[] p_data;
			p_data = nullptr;
		}
	}

	vector4<_T> operator=(const vector4<_T> &v) {
		if (this == &v)
			return *this;

		size = v.size;
		assert(size > 0);
		if (p_data) {
			delete[] p_data;
			p_data = nullptr;
		}

		p_data = new int[size];
		for (int i = 0; i < size; i++)
			p_data[i] = v.p_data[i];

		return *this;
	}

	int &operator[](int idx) {
		assert(idx >= 0 && idx < size);
		return p_data[idx];
	}

	vector4<_T> operator*(const _T k) {
		vector4 t(size);
		for (int i = 0; i < size; i++)
			t.p_data[i] = p_data[i] * k;

		return t;
	}

	int operator()() { return Size(); }

	int Size() const { return size; };
	int *begin() const { return &p_data[0]; }
	int *end() const { return &p_data[size]; }

	friend std::ostream &operator<<(std::ostream &out, const vector4<_T> &vec)
	{
		std::cout << "\nvector elements:\n";
		for (int i = 0; i < vec.size; i++)
			std::cout << i << ": " << vec.p_data[i] << "\n";

		return out;
	}

	friend std::istream &operator>>(std::istream &in, vector4<_T> &vec)
	{
		std::cout << "Fill vector:\n";
		for (int i = 0; i < vec.size; i++)
			std::cin >> vec.p_data[i];

		std::cout << "\n";
		return in;
	}
};
#include "vector2.h"

vector2::vector2(int s)
{
	//std::cout << __FUNCSIG__ " called for object " << this << "\n";
	size = s;
	assert(size > 0);
	p_data = new int[size];
	assert(p_data);
	beginit.elem = p_data;
	endit.elem = p_data;
}

vector2::vector2(int s, int v)
{
	//std::cout << __FUNCSIG__ " called for object " << this << "\n";
	size = s;
	assert(size > 0);

	p_data = new int[size];
	assert(p_data);
	for (int i = 0; i < size; i++)
		p_data[i] = v;

	beginit.elem = p_data;
	endit.elem = &p_data[size];
}

vector2::~vector2()
{
}

vector2 vector2::operator=(const vector2 &v)
{
	if (this == &v)
		return *this;

	size = v.size;
	assert(size > 0);
	if (p_data) {
		delete[] p_data;
		p_data = nullptr;
	}

	p_data = new int[size];
	assert(p_data);
	for (int i = 0; i < size; i++)
		p_data[i] = v.p_data[i];

	beginit.elem = v.beginit.elem;
	endit.elem = v.endit.elem;
	return *this;
}

int &vector2::operator[](int idx)
{
	assert(idx >= 0 && idx < size);
	return p_data[idx];
}

vector2 vector2::operator*(const int i)
{
	vector2 t(size);
	for (int i = 0; i < size; i++)
		t.p_data[i] = p_data[i] * i;

	return t;
}

int vector2::operator()()
{
	return Size();
}

std::ostream &operator<<(std::ostream &out, const vector2 &vec)
{
	std::cout << "\nvector elements:\n";
	for (int i = 0; i < vec.size; i++)
		std::cout << i << ": " << vec.p_data[i] << "\n";

	return out;
}

std::istream &operator>>(std::istream &in, vector2 &vec)
{
	std::cout << "Fill vector:\n";
	for (int i = 0; i < vec.size; i++)
		std::cin >> vec.p_data[i];

	std::cout << "\n";
	return in;
}

iter iter::operator-(int n)
{
	elem -= n;
	iter temp;
	temp.elem = elem;
	return temp;
}
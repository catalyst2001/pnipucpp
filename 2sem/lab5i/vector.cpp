#include "vector.h"

vector::vector(int sz)
{
	cur = 0;
	size = sz;
	elems = new base*[size];
}

vector::~vector()
{
	if (elems)
		delete[] elems;
}

void vector::add(base *e)
{
	if (cur < size) {
		elems[cur] = e;
		cur++;
	}
}

std::ostream &operator<<(std::ostream &out, const vector &v)
{
	if (!v.size || !v.cur) {
		out << "vector is empty!\n";
		return out;
	}

	std::cout << "\n---- Vector elements ----\n";
	base **b = v.elems;
	for (int i = 0; i < v.cur; i++) {
		(*b)->show();
		b++;
	}
	return out;
}
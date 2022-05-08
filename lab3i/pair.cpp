#include "pair.h"

bool pair::operator>(const pair &p)
{
	return (one > p.one && two > p.two);
}

bool pair::operator<(const pair & p)
{
	return (one < p.one && two < p.two);
}

pair &pair::operator++()
{
	one++;
	return *this;
}

pair pair::operator++(int)
{
	two++;
	return *this;
}

std::istream &operator>>(std::istream &in, pair &p)
{
	std::cout << "enter one number: ";
	std::cin >> p.one;
	std::cout << "enter two number: ";
	std::cin >> p.two;
	return in;
}

std::ostream &operator<<(std::ostream &out, const pair &p)
{
	return out << p.one << ":" << p.two << std::endl;
}

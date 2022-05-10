#include "pair2.h"

std::istream &operator>>(std::istream &i, pair2 &p)
{
	std::cout << "enter first: ";
	std::cin >> p.first;
	std::cout << "enter second: ";
	std::cin >> p.second;
	return i;
}

std::ostream &operator<<(std::ostream &out, const pair2 &p)
{
	return out << p.first << ":" << p.second << std::endl;
}

pair2 pair2::operator+(const pair2 &p)
{
	pair2 result(first + second, p.first + p.second);
	std::cout << "pair2 pair2::operator+(const pair2 &p) " << result << std::endl;
	return result;
}
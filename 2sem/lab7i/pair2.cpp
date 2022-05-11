#include "pair2.h"

std::istream &operator>>(std::istream &in, pair2 &p)
{
	std::cout << "enter first number for pair: ";
	std::cin >> p.first;
	std::cout << "enter next number for pair: ";
	std::cin >> p.next;
	return in;
}

pair2 pair2::operator+(const pair2 &p)
{
	return pair2(first + next, p.first + p.next);
}

std::ostream &operator<<(std::ostream &out, const pair2 &p)
{
	std::cout << "pair: " << p.first << " : " << p.next << "\n";
	return out;
}
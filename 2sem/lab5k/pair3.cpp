#include "pair3.h"

std::istream &operator>>(std::istream &i, pair3 &p)
{
	std::cout << "enter first: ";
	std::cin >> p.first;
	std::cout << "enter second: ";
	std::cin >> p.second;
	return i;
}

std::ostream &operator<<(std::ostream &out, const pair3 &p)
{
	return out << p.first << ":" << p.second << std::endl;
}

void pair3::show()
{
	std::cout << __FUNCSIG__ " first: " << first << " second: " << second << "\n";
}

pair3 pair3::operator+(const pair3 &p)
{
	pair3 result(first + second, p.first + p.second);
	std::cout << "pair2 pair2::operator+(const pair2 &p) " << result << std::endl;
	return result;
}
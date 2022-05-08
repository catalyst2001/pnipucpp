#include "fraction.h"

void fraction::init(double f, double s)
{
	first = f;
	second = s;
}

void fraction::read()
{
	std::cout << "Enter first, second: ";
	std::cin >> first >> second;
}

void fraction::show()
{
	std::cout << "First: " << first << " Second: " << second << std::endl;
}

bool fraction::range_check(double x)
{
	return first <= x && second >= x;
}

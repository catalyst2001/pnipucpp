#include "fraction.h"

void fraction::init(int f, double s)
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

double fraction::cost()
{
	return first * second;
}

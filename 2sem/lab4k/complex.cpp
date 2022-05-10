#include "complex.h"
#include "complex.h"

complex complex::operator*(const complex &c)
{
	complex result(first * c.first - second * c.second, first * c.second + second * c.first);
	std::cout << "complex::operator*(const complex &c): " << result << std::endl;
	return result;
}

complex complex::operator-(const complex &c)
{
	complex result(first - second, c.first - c.second);
	std::cout << "complex::operator-(const complex &c): " << result << std::endl;
	return result;
}

std::istream &operator>>(std::istream &i, complex &p)
{
	std::cout << "real part: ";
	std::cin >> p.first;
	std::cout << "imaginary part: ";
	std::cin >> p.second;
	return i;
}

std::ostream &operator<<(std::ostream &out, const complex &p)
{
	return out << p.first << ":" << p.second << std::endl;
}
#include "complex2.h"

void complex2::show()
{
	std::cout << __FUNCSIG__ " real_part: " << real_part << " imaginary_part: " << imaginary_part << "\n";
}

complex2 complex2::operator*(const complex2 &c)
{
	complex2 result(
		real_part * c.real_part - imaginary_part * c.imaginary_part,
		real_part * c.imaginary_part + imaginary_part * c.real_part
	);
	std::cout << "complex::operator*(const complex &c): " << result << std::endl;
	return result;
}

complex2 complex2::operator-(const complex2 &c)
{
	complex2 result(real_part - imaginary_part, c.real_part - c.imaginary_part);
	std::cout << "complex::operator-(const complex &c): " << result << std::endl;
	return result;
}

std::istream &operator>>(std::istream &i, complex2 &p)
{
	std::cout << "real part: ";
	std::cin >> p.real_part;
	std::cout << "imaginary part: ";
	std::cin >> p.imaginary_part;
	return i;
}

std::ostream &operator<<(std::ostream &out, const complex2 &p)
{
	return out << p.real_part << " : " << p.imaginary_part << std::endl;
}
// lab1double.cpp
// (a + b)^3 - (a^3)
// -----------------
// 3ab^2 + b^3 + 3a^2 * b

#include <iostream>
#include <math.h>

int main()
{
	double a = 1000.0;
	double b = 0.0001;

	double chisl = pow(a + b, 3) - pow(a, 3);
	double znamn = pow(3.0 * a * b, 2) + pow(b, 3) + 3.0 * pow(a, 2) * b;
	std::cout << chisl / znamn << std::endl;
	return 0;
}
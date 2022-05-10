// lab1.cpp
// (a + b)^3 - (a^3)
// -----------------
// 3ab^2 + b^3 + 3a^2 * b

#include <iostream>
#include <math.h>

int main()
{
	float a = 1000.0f;
	float b = 0.0001f;

	float chisl = pow(a + b, 3) - pow(a, 3);
	float znamn = pow(3.f * a * b, 2) + pow(b, 3) + 3.f * pow(a, 2) * b;
	std::cout << chisl / znamn << std::endl;
	return 0;
}
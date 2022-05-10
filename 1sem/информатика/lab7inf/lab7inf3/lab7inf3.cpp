#include <iostream>

#define EPSILON 0.0001
#define RANGE 10000000
#define LEFT_RANGE 0.0
#define RIGHT_RANGE 1.5

#define Func0(x) (pow(x, 5) - cos(pow(x, 2)))
#define Func1(x) ((5.0 * pow(x, 4)) + (2.0 * x) * sin(x*x))
#define Func2(x) ((20.0 * pow(x, 3)) + (4.0 * (x*x)) * cos(x*x) + (2.0 * sin(x*x)))

int main()
{
	double a = LEFT_RANGE;
	double b = RIGHT_RANGE;

	int n = 0;
	double c = b;
	if (Func0(a) * Func2(a) > 0)
		c = a;

	do {
		c = c - Func0(c) / Func1(c);
		n++;
	} while (fabs(Func0(c)) > EPSILON && n < RANGE);
	std::cout << "X = " << c << std::endl;
	return 0;
}
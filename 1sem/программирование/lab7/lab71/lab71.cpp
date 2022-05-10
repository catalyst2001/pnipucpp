#include <stdio.h>

int sub(int a, int b)
{
	return a - b;
}

double sub(double ai, double bi, double a, double b)
{
	return (a - b) + (ai - bi);
}

int main()
{
	printf("int sub(4, 2) = %d\n", sub(4, 2));
	printf("double sub(1.0, 1.0, 10.0, 5.0) = %lf\n", sub(1.0, 1.0, 10.0, 5.0));
	return 0;
}
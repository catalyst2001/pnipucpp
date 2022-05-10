// lab1i.cpp
#include <iostream>
#include "fraction.h"

fraction makeFraction(int f, double s)
{
	fraction fr;
	fr.init(f, s);
	return fr;
}

int main()
{
	fraction a, b;
	a.read(); a.show();
	b.read(); b.show();
	std::cout << "Sum: " << a.cost() << "\n";
	std::cout << "Sum: " << b.cost() << "\n";

	std::cout << "\n\n";
	fraction *X = new fraction;
	X->read();
	delete X;

	std::cout << "\n\n";
	int i;
	fraction arr[3];
	for (i = 0; i < 3; i++)
		arr[i].read();

	for (i = 0; i < 3; i++)
		arr[i].show();

	for (i = 0; i < 3; i++)
		std::cout << "arr[" << i << "] Sum: " << arr[i].cost() << "\n";

	std::cout << "\n\n";
	fraction *Y = new fraction[3];
	for (i = 0; i < 3; i++)
		Y[i].read();

	for (i = 0; i < 3; i++)
		Y[i].show();

	for (i = 0; i < 3; i++)
		std::cout << "arr[" << i << "] Sum: " << Y[i].cost() << "\n";

	delete[] Y;

	int f;
	double s;
	std::cout << "\n\n";
	std::cout << "First: ";
	std::cin >> f;
	std::cout << "Second: ";
	std::cin >> s;
	fraction newFratcion = makeFraction(f, s);
	newFratcion.show();
	std::cout << newFratcion.cost() << "\n";
}
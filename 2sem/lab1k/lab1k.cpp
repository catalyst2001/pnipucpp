// lab1k.cpp
//
#include <iostream>
#include "fraction.h"

void in_range_print(fraction *p_fr, double x)
{
	std::cout << "Number " << x << " " << (p_fr->range_check(x) ? "In range" : "Out of range") << std::endl;
}

fraction make_fraction(double f, double s)
{
	fraction fr;
	fr.init(f, s);
	return fr;
}

int main()
{
	double x;
	std::cout << "------ fraction a, b ------" << std::endl;
	fraction a, b;
	a.read();
	b.read();
	std::cout << "Please enter X: ";
	std::cin >> x;
	in_range_print(&a, x);
	in_range_print(&b, x);


	std::cout << std::endl << "------ fraction heap ------" << std::endl;
	fraction *p_frn = new fraction;
	p_frn->read();
	in_range_print(p_frn, x);
	delete p_frn;


	std::cout << std::endl << "------ fraction array ------" << std::endl;
	size_t i;
	fraction arr[3];
	for (i = 0; i < 3; i++)
		arr[i].read();

	for (i = 0; i < 3; i++)
		arr[i].show();

	for (i = 0; i < 3; i++) {
		std::cout << "arr[" << i << "] ";
		in_range_print(&arr[i], x);
	}


	std::cout << std::endl << "------ fraction dynamic array ------" << std::endl;
	p_frn = new fraction[3];
	for (i = 0; i < 3; i++)
		p_frn[i].read();

	for (i = 0; i < 3; i++)
		p_frn[i].show();

	for (i = 0; i < 3; i++) {
		std::cout << "arr[" << i << "] ";
		in_range_print(&p_frn[i], x);
	}
	delete[] p_frn;

	std::cout << std::endl << "------ make_fraction call ------" << std::endl;
	double f, s;
	std::cout << "First: ";
	std::cin >> f;
	std::cout << "Second: ";
	std::cin >> s;
	a = make_fraction(f, s);
	a.show();
	in_range_print(&a, x);
}
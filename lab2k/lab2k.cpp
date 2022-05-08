// lab2k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "chislo.h"

chislo make_chislo()
{
	chislo c;

	int p;
	float m;
	std::string s;
	std::cout << "Enter poryadok: ";
	std::cin >> p;
	c.set_poryadok(p);

	std::cout << "Enter mantissa: ";
	std::cin >> m;
	c.set_mantissa(m);

	std::cout << "Enter string: ";
	std::cin >> s;
	c.set_strimage(s);

	return c;
}

void print_chislo(chislo &c)
{
	c.show();
}

int main()
{
	chislo c1;
	c1.show();

	chislo c2(1, 0.20f, "1.20");
	c2.show();

	chislo c3 = c2;
	c3.set_poryadok(10);
	c3.set_mantissa(0.40f);
	c3.set_strimage("10.40f");
	print_chislo(c3);

	c1 = make_chislo();
	c1.show();
	return 0;
}
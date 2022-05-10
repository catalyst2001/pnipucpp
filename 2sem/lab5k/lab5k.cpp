// lab5k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "pair3.h"
#include "complex2.h"
#include "my_vector.h"

int main()
{
	// pair3 object
	pair3 p1;
	std::cin >> p1;
	std::cout << p1;

	IObject *p_pbjectp1 = &p1;
	std::cout << "call show() from IObject for p1\n";
	p_pbjectp1->show();

	// complex2 object
	complex2 c1;
	std::cin >> c1;
	std::cout << c1;

	IObject *p_objectc1 = &c1;
	p_objectc1->show();

	std::cout << "\nmy vector\n";
	my_vector vec(5);
	vec.add(p_pbjectp1);
	vec.add(p_objectc1);
	std::cout << vec;
	return 0;
}
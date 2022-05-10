// lab4k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "complex.h"

int main()
{
	complex c1, c2;
	std::cin >> c1;
	std::cin >> c2;

	std::cout << "complex1 + complex2 = " << c1 + c2 << std::endl;
	std::cout << "complex1 * complex2 = " << c1 * c2 << std::endl;
	std::cout << "complex1 - complex2 = " << c1 - c2 << std::endl;

	return 0;
}
// lab3i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "pair.h"

int main()
{
	pair pair1;
	std::cin >> pair1;
	std::cout << pair1;

	pair pair2;
	std::cin >> pair2;
	std::cout << pair2;

	std::cout << "operator< (pair1 < pair2): " << (pair1 < pair2 ? "TRUE" : "FALSE") << "\n";
	std::cout << "operator> (pair1 > pair2): " << (pair1 < pair2 ? "TRUE" : "FALSE") << "\n";
	std::cout << "\n";

	std::cout << "operator++\n";
	pair1++;
	std::cout << pair1;

	std::cout << "++operator\n";
	++pair1;
	std::cout << pair1;
	return 0;
}
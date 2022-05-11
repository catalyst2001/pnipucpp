// lab7i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "pair2.h"
#include "list.h"

int main()
{
	list<pair2> l1;
	for(int i = 0; i < 10; i++)
		l1.push(pair2(i, 0.));

	std::cout << "\noperator[](idx) test:\n";
	for (int i = 0; i < l1.Size(); i++)
		std::cout << l1[i];

	list<pair2> dst = l1 + pair2(10, 10.);
	for (int i = 0; i < dst.Size(); i++)
		std::cout << dst[i];

	return 0;
}
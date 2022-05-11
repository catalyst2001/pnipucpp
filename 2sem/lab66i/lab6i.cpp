// lab6i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "list.h"

int main()
{
	list l1;
	for(int i = 0; i < 10; i++)
		l1.push(i);

	std::cout << "\niterator test:\n";
	for (listiter it = l1.frist(); it != l1.last(); it++)
		std::cout << *it << " ";
	std::cout << "\n";

	std::cout << "\noperator[](idx) test:\n";
	for (int i = 0; i < l1.Size(); i++)
		std::cout << l1[i] << " ";

	std::cout << "\noperator+(int c) test:\n";
	list res = l1 + 0;
	for (int i = 0; i < l1.Size(); i++)
		std::cout << res[i] << " ";

	return 0;
}
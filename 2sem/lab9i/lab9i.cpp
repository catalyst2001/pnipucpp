// lab9i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "list.h"

int main()
{
	std::cout <<
		"1. index out of range\n"
		"2. init with null size\n"
		"3. exceeding the maximum size\n\n";

	int c;
	cin >> c;
	switch (c) {
	case 1:
		try {
			list list;
			for (int i = 0; i < 15; i++)
				++list;
		}
		catch (Error &exception)
		{
			exception.what();
			return 1;
		}
		break;

	case 2: {
		try {
			list list(0);
		}
		catch (Error &exception)
		{
			exception.what();
			return 1;
		}
		break;
	}

	case 3: {
		try {
			list list(15);
		}
		catch (Error &exception)
		{
			exception.what();
			return 1;
		}
		break;
	}

	default:
		cout << "Invalid menu item!\n";
	}
	return 0;
}
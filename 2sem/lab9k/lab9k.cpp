// lab9k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "vector10.h"

int main()
{
	std::cout <<
		"1. index out of range\n"
		"2. init with null size\n"
		"3. remove from vector begin with null size\n"
		"4. remove from vector end with null size\n"
		"5. exceeding the maximum size\n\n";

	int c;
	cin >> c;
	switch (c) {
	case 1:
		try {
			vector10 vec1(10);
			vec1[11] = 1;
		}
		catch (Error &exception)
		{
			exception.what();
			return 1;
		}
		break;

	case 2: {
		try {
			vector10 vec1(0);
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
			vector10 vec1(0);
			vec1--;
		}
		catch (Error &exception)
		{
			exception.what();
			return 1;
		}
		break;
	}

	case 4: {
		try {
			vector10 vec1(0);
			--vec1;
		}
		catch (Error &exception)
		{
			exception.what();
			return 1;
		}
		break;
	}

	case 5: {
		try {
			vector10 vec1(31);
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
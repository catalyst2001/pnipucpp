﻿// lab6k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "vector2.h"

int main()
{
	// test 1
	std::cout << "Test 1. input/output from streams\n";
	vector2 vec1(5);
	std::cin >> vec1;
	std::cout << vec1;

	// test 2
	std::cout << "Test 2. initializing values\n";
	vector2 vec2(10, 0);
	std::cout << vec2 << "\n";

	// test 3
	std::cout << "Test 3. test maual init && get size\n";
	vector2 vec4(5);
	for (int i = 0; i < vec4(); i++)
		vec4[i] = i;

	std::cout << vec4 << "\n";

	// test 4
	std::cout << "Test 4. test range for\n";
	for (auto i : vec1)
		std::cout << i << " ";

	std::cout << "\n";

	// test 5 
	std::cout << "Test 5. Iterators\n";
	for (iter it = vec1.first(); it != vec1.last(); it++)
		std::cout << *it << " ";

	return 0;
}
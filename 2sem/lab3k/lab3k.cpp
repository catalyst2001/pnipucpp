// lab3k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "time.h"

int main()
{
	time time1, time2;
	std::cout << "Enter time1: ";
	std::cin >> time1;

	std::cout << "Enter time2: ";
	std::cin >> time2;
	
	std::cout << "Operator== for time1 and time2: " << ((time1 == time2) ? "TRUE" : "FALSE") << std::endl;
	std::cout << "Operator!= for time1 and time2: " << ((time1 != time2) ? "TRUE" : "FALSE") << std::endl;
	return 0;
}
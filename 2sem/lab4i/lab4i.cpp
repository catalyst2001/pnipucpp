// lab4i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "student.h"

int main()
{
	person person1;
	std::cin >> person1;
	std::cout << person1;

	person person2(person1);
	std::cout << person2;

	student student1;
	std::cin >> student1;
	std::cout << student1;

	student student2;
	std::cin >> student2;
	std::cout << student2;

	// operator=
	student1 = student2;

	std::cout << "\nget and print person from student object\n";
	std::cout << student1.getPerson() << "\n";
	return 0;
}

// lab5i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "base.h"
#include "person.h"
#include "student.h"
#include "vector.h"

int main()
{
	person pers1;
	std::cin >> pers1;
	base *ptrp = &pers1;
	ptrp->show();

	student stud;
	std::cin >> stud;
	stud.incLearnYear();

	std::cout << "\nset learn year to 1 from setLearnYear\n";
	stud.setLearnYear(1);
	base *ptrs = &stud;
	ptrs->show();

	vector v(4);
	v.add(ptrp);
	v.add(ptrs);
	std::cout << v;
	return 0;
}
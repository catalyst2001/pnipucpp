// lab8k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "Tree.h"
#include "Dialog.h"
#include "Event.h"

int main()
{
	Person *a1 = new Person;
	a1->Input();
	a1->Show();

	Emloyee *b1 = new Emloyee;
	b1->Input();
	b1->Show();

	Person *a2 = new Person;//создаем объект класса Person
	a2->Input();
	Emloyee *b2 = new Emloyee; //создаем объект класса Emloyee
	b2->Input();

	Tree v(10);//Создаем вектор
	Object*p = a2;//ставим указатель на объект класса 
	v.Add(p);//добавляем объект в вектор
	p = b2;//ставим указатель на объект класса Emloyee
	v.Add(p); //добавляем объект в вектор
	v.Show();//вывод вектора
	v.Del();//удаление элемента
	cout << "\nVector size=" << v();

	cout << "events\n";
	Dialog D;
	D.Execute();
	return 0;
}
// lab8k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "vector11.h"
#include "Dialog.h"

#include "Event.h"

//#define WITH_TESTS

int main()
{
#ifdef WITH_TESTS
	cout << "iface\n";
	{
		Print *a = new Print;
		a->Input();
		a->Show();

		Book *b = new Book;
		a->Input();
		a->Show();
	}

	cout << "add to vector\n";
	{
		Print *a = new Print;//создаем объект класса Print
		a->Input();
		Book *b = new Book; //создаем объект класса Book
		b->Input();

		Vector11 v(10);//Создаем вектор
		Object*p = a;//ставим указатель на объект класса Cardf
		v.Add(p);//добавляем объект в вектор
		p = b;//ставим указатель на объект класса Book
		v.Add(p); //добавляем объект в вектор
		v.Show();//вывод вектора
		v.Del();//удаление элемента
		cout << "\nVector size=" << v();
	}
#endif
	cout << "events\n";
	Dialog D;
	D.Execute();
	return 0;
}
/*
4. Удалить максимальный элемент из словаря.
5. Каждый элемент разделить на минимальное значение словаря.
6. Выполнение всех заданий оформить в виде методов параметризированного класса
*/
#include <iostream>
#include "list.h"
#include "Container.h"

using namespace std;
int main()
{
	int n; //количество элементов в контейнере
	cout << "N?"; cin >> n;
	Container <int> v(n);//создать контенер
	v.Print();//напечатать контейнер

	int t = v.Srednee();//найти среднее арифметическое
	cout << "Srednee=" << t << endl;
	cout << "Add srednee" << endl;
	cout << "pos?";
	int pos;
	cin >> pos;//позиция для добавления
	v.Add(pos, t);//добавление
	v.Print();//печать
	
	return 0;
}
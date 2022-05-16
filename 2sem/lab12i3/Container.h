#pragma once
#include <iostream>
#include <vector> //словарь
#include <limits.h>
#include "list.h"
using namespace std;

//параметризированный класс
template<class T>
class Container
{
	list<T> v; //контейнер словарь
	int len; //длина словаря
public:
	Container(void); //конструктор без параметров
	Container(int n); //конструктор с параметрами
	void Print(); //печать
	~Container(void); //деструктор
	T &operator[](int idx) { return v[idx]; };
	int Srednee();
	void Add(int n, T el);
	int Min();
};

//реализация методов
//конструктор без параметров
template <class T>
Container<T>::Container()
{
	len = 0;
}

//деструктор
template <class T>
Container<T>::~Container(void)
{
}

//конструктор c параметрами
template <class T>
Container<T>::Container(int n)
{
	T a;
	for (int i = 0;i < n;i++)
	{
		cin >> a;
		v.push(a);
	}
	len = v.Size();
}

//вывод контейнера
template <class T>
void Container<T>::Print()
{
	for (int i = 0;i < v.Size(); i++)
		cout << i << " - " << v[i] << " " << endl;
	cout << endl;
}

//вычисление среднего арифметического
template<class T>
int Container<T>::Srednee()
{
	int s = v[0]; //начальное значение суммы – первый элемент словаря
	//перебор словаря
	for (int i = 1;i < v.Size();i++)
		s = s + v[i];
	int n = v.Size();//количество элементов в словаре
	return s / n;
}

//добавление
template<class T>
void Container<T>::Add(int n, T el)
{
	v.push(n);//формируем пару и добавляем ее в словарь
}

template<class T>
int Container<T>::Min()
{
	int nom = 0, k = 0;
	int lastTime = INT_MAX;
	for (int i = 1; i < v.Size(); i++) {
		int curTime = v[i].get_sec() + v[i].get_min() * 60;
		if (curTime < lastTime) {
			lastTime = curTime;
			nom = i;
		}
	}
	return nom;//номер min
}
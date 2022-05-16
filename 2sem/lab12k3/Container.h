#pragma once
#include <iostream>
#include <vector> //словарь
#include <limits.h>
using namespace std;

//параметризированный класс
template<class T>
class Container
{
	vector<T> v; //контейнер словарь
	int len; //длина словаря
public:
	Container(void); //конструктор без параметров
	Container(int n); //конструктор с параметрами
	void Print(); //печать
	~Container(void); //деструктор
	T &operator[](int idx) { return v[idx]; };
	T Srednee();
	void Add(int n, T el);
	int Max() {
		int nom = 0, k = 0;
		int lastTime = 0;
		for (int i = 1; i < v.size(); i++) {
			int curTime = v[i].get_sec() + v[i].get_min() * 60;
			if (curTime > lastTime) {
				lastTime = curTime;
				nom = i;
			}
		}
		return nom;//номер max
	}
	void Del();
	int Min();
	void Delenie();
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
	v.reserve(n);
	for (int i = 0;i < n;i++)
	{
		cin >> a;
		v.push_back(a);
	}
	len = v.size();
}

//вывод контейнера
template <class T>
void Container<T>::Print()
{
	for (int i = 0;i < v.size(); i++)
		cout << i << " - " << v[i] << " " << endl;
	cout << endl;
}

//вычисление среднего арифметического
template<class T>
T Container<T>::Srednee()
{
	myTime s = v[0]; //начальное значение суммы – первый элемент словаря
	//перебор словаря
	for (int i = 1;i < v.size();i++)
		s = s + v[i];
	int n = v.size();//количество элементов в словаре
	return s / n;
}

//добавление
template<class T>
void Container<T>::Add(int n, T el)
{
	v.insert(v.begin()+n, el);//формируем пару и добавляем ее в словарь
}

template <class T>
void Container<T>::Del()
{
	int max = Max();
	cout << "max=" << v[max] << " nom=" << max << endl;
	v.erase(v.begin()+max);//удалить максимальный элемент
}

template<class T>
int Container<T>::Min()
{
	int nom = 0, k = 0;
	int lastTime = INT_MAX;
	for (int i = 1; i < v.size(); i++) {
		int curTime = v[i].get_sec() + v[i].get_min() * 60;
		if (curTime < lastTime) {
			lastTime = curTime;
			nom = i;
		}
	}
	return nom;//номер min
}

template<class T>
void Container<T>::Delenie()
{
	T m = v[Min()];//найти минимальный элемент
	cout << "Min= " << m << endl;
	for (int i = 0;i < v.size();i++)
		v[i] = v[i] / m;
}
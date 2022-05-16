#include <iostream>
#include <map>
#include "myTime.h"
using namespace std;

typedef map<int, myTime>TMap;//определяем тип для работы со словаре
typedef TMap::iterator it;

//функция для формирования словаря
TMap make_map(int n)
{
	TMap m;//пустой словарь
	myTime a;
	for (int i = 0;i < n;i++)
	{
		cin >> a;
		//создаем пару и добавляем ее в словарь
		m.insert(make_pair(i, a));
	}
	return m;//возвращаем вектор как результа работы функции
}

//функция для печати словаря
void print_map(TMap m)
{
	for (int i = 0;i < m.size();i++)
		cout << i << " - " << m[i] << " " << endl;
}

//вычисление среднего арифметического
myTime srednee(TMap m)
{
	myTime s = m[0];//первый элемент – начальное значение суммы
	//перебор словаря
	for (int i = 1;i < m.size();i++)
		s = s + m[i];
	int n = m.size();//количество элементов в словаре
	return s / n;
}

//поиск максимального элемента
int Max(TMap v)
{
	it i = v.begin();int nom = 0, k = 0;
	myTime m = (*i).second;//значение первого элемента
	while (i != v.end())
	{
		if (m < (*i).second)
		{
			m = (*i).second;
			nom = k;
		}
		i++;//итератор
		k++;//счетчик элементов
	}
	return nom;//номер max
}

//поиск минимального элемента
int Min(TMap v)
{
	it i = v.begin();int nom = 0, k = 0;
	myTime m = (*i).second;//значение первого элемента
	while (i != v.end())
	{
		if (m > (*i).second)
		{
			m = (*i).second;
			nom = k;
		}
		i++;//итератор
		k++;//счетчик элементов
	}
	return nom;//номер max
}

void delenie(TMap &v)
{
	myTime m = v[Min(v)];
	for (int i = 0;i < v.size();i++)
		v[i] = v[i] / m;
}

void main()
{
	int n, k;
	cout << "K?"; cin >> k;
	cout << "N?"; cin >> n;
	map<int, myTime> m = make_map(n);
	print_map(m);

	//вычисление среднего
	myTime el = srednee(m);
	cout << "srednee=" << el << endl;

	//добавление под номером n
	m.insert(make_pair(n, el));
	print_map(m);

	int max = Max(m);
	cout << "max=" << m[max] << " nom=" << max << endl;
	m.erase(max);
	print_map(m);

	int min = Min(m);
	cout << "min=" << m[min] << " nom=" << min << endl;
	delenie(m);
	print_map(m);
}
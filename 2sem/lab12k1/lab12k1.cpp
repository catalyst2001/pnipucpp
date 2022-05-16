#include <iostream>
#include <map>
using namespace std;
typedef map<int, int>TMap;//определяем тип для работы со словарем
typedef TMap::iterator it;//итератор

//функция для формирования словаря
TMap make_map(int n)
{
	TMap m;//пустой словарь
	int a;
	for (int i = 0;i < n;i++)
	{
		cout << "?";
		cin >> a;
		//создаем пару и добавляем ее в словарь
		m.insert(make_pair(i, a));
	}
	return m;//возвращаем словарь как результат работы функции
}

//функция для печати словаря
void print_map(TMap m)
{
	for (int i = 0;i < m.size();i++)
		cout << i << " : " << m[i] << " " << endl;
}

//функция для добавления числа к элементам контейнера
void add_to_map(TMap &m, int k)
{
	for (int i = 0;i < m.size();i++)
		m[i] += k;
}
//вычисление среднего арифметического
int srednee(TMap v)
{
	int s = 0;
	//перебор словаря
	for (int i = 0;i < v.size();i++)
		s += v[i];
	int n = v.size();//количество элементов в словаре
	return s / n;
}

// поиск максимального элемента
int Max(TMap v)
{
	it i = v.begin();
	int nom = 0,//номер максимального
		k = 0;//счетчик элементов
	int m = (*i).second;//значение первого элемента
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

int Min(TMap v)
{
	it i = v.begin();int nom = 0, k = 0;
	int m = (*i).second;//значение первого элемента
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
	int m = v[Min(v)];//значение минимального элемента
	for (int i = 0;i < v.size();i++)
		v[i] = v[i] / m;
}

//основная функция
void main()
{
	int n;
	cout << "N?"; cin >> n;//количество элементов
	TMap m = make_map(n);//создать словарь
	print_map(m);//напечатать словарь

	//вычисление среднего
	int el = srednee(m);
	cout << "srednee=" << el << endl;
	add_to_map(m, 5); //задание 5
	print_map(m);

	//добавление в конец
	cout << "new" << endl;
	m.insert(make_pair(n, el));
	print_map(m);

	int max = Max(m);
	cout << "max=" << m[max] << " nom=" << max << endl;
	m.erase(max);//удаление элемента (задание 4)
	print_map(m);

	int min = Min(m);
	cout << "min=" << m[min] << " nom=" << min << endl;
	delenie(m);
	print_map(m);
}
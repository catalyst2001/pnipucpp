#include <iostream>
#include <map>
using namespace std;

typedef pair<int, int> myPair;//определяем тип для работы со словаре
typedef map<int, myPair> TMap;//определяем тип для работы со словаре
typedef TMap::iterator it;

//функция для формирования словаря
TMap make_map(int n)
{
	TMap m;//пустой словарь
	myPair a;
	for (int i = 0;i < n;i++)
	{
		cin >> a.first; //доп.ключ
		cin >> a.second;//значение
		//создаем пару и добавляем ее в словарь
		m.insert(make_pair(i, a));
	}
	return m;//возвращаем вектор как результа работы функции
}

//функция для печати словаря
void print_map(TMap m)
{
	for (int i = 0;i < m.size();i++)
		cout << i << " - " << m[i].first << " , " << m[i].second << " " << endl;
}

//вычисление среднего арифметического
int srednee(TMap m)
{
	int s = m[0].second;//первый элемент – начальное значение суммы
	//перебор словаря
	for (int i = 1;i < m.size();i++)
		s += m[i].second;
	int n = m.size();//количество элементов в словаре
	return s / n;
}

//поиск максимального элемента
int Max(TMap v)
{
	it i = v.begin();int nom = 0, k = 0;
	myPair m = (*i).second;//значение первого элемента
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
	myPair m = (*i).second;//значение первого элемента
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
	myPair m = v[Min(v)];
	for (int i = 0;i < v.size();i++) {
		if (m.second == 0) m.second = 1;
		v[i].second = v[i].second / m.second;
	}
}

int main()
{
	int n;
	cout << "N?"; cin >> n;
	map<int, myPair> m = make_map(n);
	print_map(m);

	//вычисление среднего
	int el = srednee(m);
	cout << "srednee=" << el << endl;

	//добавление под номером n
	m.insert(make_pair(n, myPair(n,el)));
	print_map(m);

	int max = Max(m);
	cout << "max=" << m[max].second << " nom=" << max << endl;
	m.erase(max);
	print_map(m);

	int min = Min(m);
	cout << "min=" << m[min].second << " nom=" << min << endl;
	delenie(m);
	print_map(m);

	return 0;
}
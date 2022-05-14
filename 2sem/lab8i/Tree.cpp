#include "Tree.h"

//деструктор
Tree::~Tree(void)
{
	if (beg != 0)
		delete[] beg;

	beg = 0;
}

//конструктор с параметрами
Tree::Tree(int n)
{
	beg = new Object*[n];
	cur = 0;
	size = n;
}

//добавление объекта, на который указывает указатель p в вектор
void Tree::Add()
{
	Object*p;
	//выбор из объектов двух возможных классов
	cout << "1.Person" << endl;
	cout << "2.Emloyee" << endl;
	int y;
	cin >> y;
	if (y == 1)//добавление объекта класса Person
	{
		Person *a = new Person;
		a->Input();//ввод значений атрибутов
		p = a;
		if (cur < size)
		{
			beg[cur] = p;//добавление в вектор
			cur++;
		}
	}
	else if (y == 2) //добавление объекта класса Emloyee
	{
		Emloyee *b = new Emloyee;
		b->Input();
		p = b;
		if (cur < size)
		{
			beg[cur] = p;
			cur++;
		}
	}
	return;
}

void Tree::Add(Object *p)
{
	if (cur < size)
	{
		beg[cur] = p;//добавление в вектор
		cur++;
	}
}

//просмотр вектора
void Tree::Show()
{
	if (cur == 0)
		cout << "Empty" << endl;

	Object **p = beg;//указатель на указатель типа Object
	for (int i = 0; i < cur; i++)
	{
		(*p)->Show();//вызов метода Show() (позднее связывание)
		p++;//передвигаем указатель на следующий объект
	}
}

void Tree::ShowOne(int n)
{
	if (cur == 0) {
		cout << "Empty" << endl;
		return;
	}

	if(n < 0 || n > cur) {
		cout << "Index out of bounds\n";
		return;
	}
	beg[n]->Show();
}

//операция, которая возвращает размер вектора
int Tree::operator ()()
{
	return cur;
}

//удаление элемента из вектора, память не освобождается!
void Tree::Del()
{
	if (cur == 0)
		return;//пустой

	cur--;
}

void Tree::HandleEvent(const TEvent&e)
{
	if (e.what == evMessage)
	{
		Object**p = beg;
		for (int i = 0; i < cur; i++)
		{
			(*p)->HandleEvent(e);//вызов метода (позднее связывание)
			p++;//передвигаем указатель на следующий объект
		}
	}
}
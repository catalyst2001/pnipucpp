#include "Vector11.h"

//деструктор
Vector11::~Vector11(void)
{
	if (beg != 0)
		delete[] beg;

	beg = 0;
}

//конструктор с параметрами
Vector11::Vector11(int n)
{
	beg = new Object*[n];
	cur = 0;
	size = n;
}

//добавление объекта, на который указывает указатель p в вектор
void Vector11::Add()
{
	Object*p;
	//выбор из объектов двух возможных классов
	cout << "1.Print" << endl;
	cout << "2.Book" << endl;
	int y;
	cin >> y;
	if (y == 1)//добавление объекта класса Print
	{
		Print *a = new Print;
		a->Input();//ввод значений атрибутов
		p = a;
		if (cur < size)
		{
			beg[cur] = p;//добавление в вектор
			cur++;
		}
	}
	else if (y == 2) //добавление объекта класса Book
	{
		Book *b = new Book;
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

void Vector11::Add(Object *p)
{
	if (cur < size)
	{
		beg[cur] = p;//добавление в вектор
		cur++;
	}
}

//просмотр вектора
void Vector11::Show()
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

void Vector11::ShowOne(int n)
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
int Vector11::operator ()()
{
	return cur;
}

//удаление элемента из вектора, память не освобождается!
void Vector11::Del()
{
	if (cur == 0)
		return;//пустой

	cur--;
}

void Vector11::HandleEvent(const TEvent&e)
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
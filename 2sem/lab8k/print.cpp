#include "Print.h"

Print::Print(void)
{
	name = "";
	author = "";
}

//деструктор
Print::~Print(void)
{
}

//констрктор с параметрами
Print::Print(std::string M, std::string a)
{
	name = M;
	author = a;
}

//конструктор копирования
Print::Print(const Print& car)
{
	name = car.name;
	author = car.author;
}

//оператор присваивания
Print& Print::operator=(const Print&c)
{
	if (&c == this)return *this;
	name = c.name;
	author = c.author;
	return *this;
}

//метод для просмотра атрибутов
void Print::Show()
{
	cout << "\nNAME : " << name;
	cout << "\nAUTHOR : " << author;
}

//метод для ввода значений атрибутов
void Print::Input()
{
	cout << "\nNAME:"; cin >> name;
	cout << "\nAUTHOR:"; cin >> author;
}

void Print::HandleEvent(const TEvent &e)
{
	if (e.what == evMessage)//событие-сообщение
	{
		switch (e.command)
		{
		case cmGet:cout << "name=" << get_name() << endl;
			break;
		}
	}
}
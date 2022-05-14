#include "Person.h"

Person::Person(void)
{
	name = "";
	age = 0;
}

//деструктор
Person::~Person(void)
{
}

//констрктор с параметрами
Person::Person(std::string M, int a)
{
	name = M;
	age = a;
}

//конструктор копирования
Person::Person(const Person& car)
{
	name = car.name;
	age = car.age;
}

//оператор присваивания
Person& Person::operator=(const Person&c)
{
	if (&c == this)return *this;
	name = c.name;
	age = c.age;
	return *this;
}

//метод для просмотра атрибутов
void Person::Show()
{
	cout << "\nname : " << name;
	cout << "\nage : " << age;
}

//метод для ввода значений атрибутов
void Person::Input()
{
	cout << "\nname:"; cin >> name;
	cout << "\nage:"; cin >> age;
}

void Person::HandleEvent(const TEvent &e)
{
	if (e.what == evMessage)//событие-сообщение
	{
		switch (e.command)
		{
		case cmGet:cout << "name=" << getName() << " age=" << getAge() << endl;
			break;
		}
	}
}
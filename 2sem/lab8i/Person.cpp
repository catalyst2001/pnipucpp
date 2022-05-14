#include "Person.h"

Person::Person(void)
{
	name = "";
	age = 0;
}

//����������
Person::~Person(void)
{
}

//���������� � �����������
Person::Person(std::string M, int a)
{
	name = M;
	age = a;
}

//����������� �����������
Person::Person(const Person& car)
{
	name = car.name;
	age = car.age;
}

//�������� ������������
Person& Person::operator=(const Person&c)
{
	if (&c == this)return *this;
	name = c.name;
	age = c.age;
	return *this;
}

//����� ��� ��������� ���������
void Person::Show()
{
	cout << "\nname : " << name;
	cout << "\nage : " << age;
}

//����� ��� ����� �������� ���������
void Person::Input()
{
	cout << "\nname:"; cin >> name;
	cout << "\nage:"; cin >> age;
}

void Person::HandleEvent(const TEvent &e)
{
	if (e.what == evMessage)//�������-���������
	{
		switch (e.command)
		{
		case cmGet:cout << "name=" << getName() << " age=" << getAge() << endl;
			break;
		}
	}
}
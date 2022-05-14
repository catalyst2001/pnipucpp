#include "Print.h"

Print::Print(void)
{
	name = "";
	author = "";
}

//����������
Print::~Print(void)
{
}

//���������� � �����������
Print::Print(std::string M, std::string a)
{
	name = M;
	author = a;
}

//����������� �����������
Print::Print(const Print& car)
{
	name = car.name;
	author = car.author;
}

//�������� ������������
Print& Print::operator=(const Print&c)
{
	if (&c == this)return *this;
	name = c.name;
	author = c.author;
	return *this;
}

//����� ��� ��������� ���������
void Print::Show()
{
	cout << "\nNAME : " << name;
	cout << "\nAUTHOR : " << author;
}

//����� ��� ����� �������� ���������
void Print::Input()
{
	cout << "\nNAME:"; cin >> name;
	cout << "\nAUTHOR:"; cin >> author;
}

void Print::HandleEvent(const TEvent &e)
{
	if (e.what == evMessage)//�������-���������
	{
		switch (e.command)
		{
		case cmGet:cout << "name=" << get_name() << endl;
			break;
		}
	}
}
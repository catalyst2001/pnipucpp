#pragma once
#include "object.h"
#include "Event.h"

class Person : public Object
{
public:
	Person(void);//����������� ��� ����������
public:
	virtual ~Person(void);//����������

	void Show(); //������� ��� ��������� ��������� ������ � ������� ���������void Input();//������� ��� ����� �������� ���������
	void Input();

	Person(std::string name, int age);//����������� � �����������
	Person(const Person&);//����������� �����������

	//���������
	std::string getName() { return name; }
	int getAge() { return age; }
	
	//������������
	void setName(std::string n) { name = n; };
	void setAge(int aa) { age = aa; }
	Person& operator=(const Person&);//���������� �������� ������������
	void HandleEvent(const TEvent &e);
protected:
	std::string name;
	int age;
};
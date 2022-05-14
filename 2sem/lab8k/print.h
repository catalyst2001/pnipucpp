#pragma once
#include "object.h"
#include "Event.h"

class Print : public Object
{
public:
	Print(void);//����������� ��� ����������
public:
	virtual ~Print(void);//����������

	void Show(); //������� ��� ��������� ��������� ������ � ������� ���������void Input();//������� ��� ����� �������� ���������
	void Input();

	Print(std::string name, std::string author);//����������� � �����������
	Print(const Print&);//����������� �����������

	//���������
	std::string get_name() { return name; }
	std::string get_author() { return author; }

	//������������
	void set_name(std::string n) { name = n; };
	void set_author(std::string au) { author = au; }
	Print& operator=(const Print&);//���������� �������� ������������
	void HandleEvent(const TEvent &e);
protected:
	std::string name;
	std::string author;
};
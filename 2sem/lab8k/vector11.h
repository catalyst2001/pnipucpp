#pragma once
#include "print.h"
#include "Book.h"

class Vector11
{
public:
	Vector11() : size(0) { }
	Vector11(int);//����������� � �����������
	~Vector11(void);//����������
	void Add();//���������� �������� � ������
	void Add(Object *pp);
	void Del();
	void Show();
	void ShowOne(int n);
	int operator()();//������ �������

	void HandleEvent(const TEvent&e);
protected:
	Object **beg;//��������� �� ������ ������� �������
	int size;//������
	int cur;//������� �������
};


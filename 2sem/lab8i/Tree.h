#pragma once
#include "Person.h"
#include "Emloyee.h"

class Tree
{
public:
	Tree() {}
	Tree(int);//����������� � �����������
	~Tree(void);//����������
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


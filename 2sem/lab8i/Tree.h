#pragma once
#include "Person.h"
#include "Emloyee.h"

class Tree
{
public:
	Tree() {}
	Tree(int);//конструктор с параметрами
	~Tree(void);//деструктор
	void Add();//добавление элемента в вектор
	void Add(Object *pp);
	void Del();
	void Show();
	void ShowOne(int n);
	int operator()();//размер вектора

	void HandleEvent(const TEvent&e);
protected:
	Object **beg;//указатель на первый элемент вектора
	int size;//размер
	int cur;//текущая позиция
};


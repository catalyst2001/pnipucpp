#pragma once
#include "print.h"
#include "Book.h"

class Vector11
{
public:
	Vector11() : size(0) { }
	Vector11(int);//конструктор с параметрами
	~Vector11(void);//деструктор
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


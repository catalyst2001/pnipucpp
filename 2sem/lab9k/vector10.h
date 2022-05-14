#pragma once
#include <iostream>
#include <exception>
#include <string.h>
#include "error.h"



class vector10
{
	int m_size;
	int m_current;
	int *p_data;
public:
	vector10() : m_size(0), m_current(0), p_data(nullptr) {}
	vector10(int size);
	vector10(int *arr, int size);
	vector10(const vector10 &from);
	~vector10();

	int &operator[](int idx);
	int operator()();
	void operator--(); //prefix
	void operator--(int); //postfix
};


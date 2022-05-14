#pragma once
#include <iostream>//библиотека для работы со стандартными потоками
#include <fstream> //библиотека для работы со файловыми потоками
#include <string>
using namespace std;

class Pair
{
public:
	Pair();
	Pair(int m, int s);
	Pair(const Pair&);
	~Pair();

	friend ostream& operator<<(ostream &out, const Pair&p);
	friend istream& operator>>(istream &in, Pair &p);

	Pair operator=(const Pair&);
	bool operator==(const Pair &tm);
	bool operator!=(const Pair &tm);
	void operator++();
	void operator++(int);

	bool operator>(const Pair &tm);
	bool operator<(const Pair &tm);

	void add(int c);
private:
	int first;
	double next;
};
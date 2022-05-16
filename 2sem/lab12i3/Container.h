#pragma once
#include <iostream>
#include <vector> //�������
#include <limits.h>
#include "list.h"
using namespace std;

//������������������� �����
template<class T>
class Container
{
	list<T> v; //��������� �������
	int len; //����� �������
public:
	Container(void); //����������� ��� ����������
	Container(int n); //����������� � �����������
	void Print(); //������
	~Container(void); //����������
	T &operator[](int idx) { return v[idx]; };
	int Srednee();
	void Add(int n, T el);
	int Min();
};

//���������� �������
//����������� ��� ����������
template <class T>
Container<T>::Container()
{
	len = 0;
}

//����������
template <class T>
Container<T>::~Container(void)
{
}

//����������� c �����������
template <class T>
Container<T>::Container(int n)
{
	T a;
	for (int i = 0;i < n;i++)
	{
		cin >> a;
		v.push(a);
	}
	len = v.Size();
}

//����� ����������
template <class T>
void Container<T>::Print()
{
	for (int i = 0;i < v.Size(); i++)
		cout << i << " - " << v[i] << " " << endl;
	cout << endl;
}

//���������� �������� ���������������
template<class T>
int Container<T>::Srednee()
{
	int s = v[0]; //��������� �������� ����� � ������ ������� �������
	//������� �������
	for (int i = 1;i < v.Size();i++)
		s = s + v[i];
	int n = v.Size();//���������� ��������� � �������
	return s / n;
}

//����������
template<class T>
void Container<T>::Add(int n, T el)
{
	v.push(n);//��������� ���� � ��������� �� � �������
}

template<class T>
int Container<T>::Min()
{
	int nom = 0, k = 0;
	int lastTime = INT_MAX;
	for (int i = 1; i < v.Size(); i++) {
		int curTime = v[i].get_sec() + v[i].get_min() * 60;
		if (curTime < lastTime) {
			lastTime = curTime;
			nom = i;
		}
	}
	return nom;//����� min
}
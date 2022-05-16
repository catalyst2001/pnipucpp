#pragma once
#include <iostream>
#include <vector> //�������
#include <limits.h>
using namespace std;

//������������������� �����
template<class T>
class Container
{
	vector<T> v; //��������� �������
	int len; //����� �������
public:
	Container(void); //����������� ��� ����������
	Container(int n); //����������� � �����������
	void Print(); //������
	~Container(void); //����������
	T &operator[](int idx) { return v[idx]; };
	T Srednee();
	void Add(int n, T el);
	int Max() {
		int nom = 0, k = 0;
		int lastTime = 0;
		for (int i = 1; i < v.size(); i++) {
			int curTime = v[i].get_sec() + v[i].get_min() * 60;
			if (curTime > lastTime) {
				lastTime = curTime;
				nom = i;
			}
		}
		return nom;//����� max
	}
	void Del();
	int Min();
	void Delenie();
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
	v.reserve(n);
	for (int i = 0;i < n;i++)
	{
		cin >> a;
		v.push_back(a);
	}
	len = v.size();
}

//����� ����������
template <class T>
void Container<T>::Print()
{
	for (int i = 0;i < v.size(); i++)
		cout << i << " - " << v[i] << " " << endl;
	cout << endl;
}

//���������� �������� ���������������
template<class T>
T Container<T>::Srednee()
{
	myTime s = v[0]; //��������� �������� ����� � ������ ������� �������
	//������� �������
	for (int i = 1;i < v.size();i++)
		s = s + v[i];
	int n = v.size();//���������� ��������� � �������
	return s / n;
}

//����������
template<class T>
void Container<T>::Add(int n, T el)
{
	v.insert(v.begin()+n, el);//��������� ���� � ��������� �� � �������
}

template <class T>
void Container<T>::Del()
{
	int max = Max();
	cout << "max=" << v[max] << " nom=" << max << endl;
	v.erase(v.begin()+max);//������� ������������ �������
}

template<class T>
int Container<T>::Min()
{
	int nom = 0, k = 0;
	int lastTime = INT_MAX;
	for (int i = 1; i < v.size(); i++) {
		int curTime = v[i].get_sec() + v[i].get_min() * 60;
		if (curTime < lastTime) {
			lastTime = curTime;
			nom = i;
		}
	}
	return nom;//����� min
}

template<class T>
void Container<T>::Delenie()
{
	T m = v[Min()];//����� ����������� �������
	cout << "Min= " << m << endl;
	for (int i = 0;i < v.size();i++)
		v[i] = v[i] / m;
}
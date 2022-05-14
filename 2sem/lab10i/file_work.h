#pragma once
#include "Pair.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int make_file(const char* f_name)
{
	fstream stream(f_name, ios::out | ios::trunc);//������� ��� ������
	if (!stream)
		return -1;//������ �������� �����

	int n;
	Pair p;
	cout << "N?";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> p;//���� ��������� ������� �� ������������ ������
		stream << p << "\n";//������ ������� � �������� �����
	}
	stream.close();//������� �����
	return n;//������� ���������� ���������� ��������
}

int print_file(const char*f_name)
{
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)return -1;//������ �������� �����
	Pair p; int i = 0;
	while (stream >> p)
	{
		cout << p << "\n";
		i++;
	}
	stream.close();
	return i;
}

int del_file(const char*f_name, int k)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)return -1;//������ �������� �����
	int i = 0; Pair p;
	while (stream >> p)//���� ��� ����� ����� ��������� ������ �������
	{
		//���� �������� ������� ����� �����,�� ����� �� �����
		if (stream.eof())
			break;

		i++;
		//���� ����� ������� �� ����� k, �� ���������� ��� �� �������������� ����
		if (i != k)
			temp << p;
	}
	//������� �����
	stream.close();
	temp.close();
	remove(f_name);//������� ������ ����
	rename("temp", f_name);// ������������� temp
	return i;//���������� �����������
}

int add_file(const char*f_name, int k, Pair pp)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)return -1;//������ �������� �����
	Pair p; int i = 0, l = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		i++;
		if (i == k)
		{
			temp << pp;//�������� � temp ����� ������
			l++;
		}
		temp << p;
	}
	stream.close(); temp.close();
	remove(f_name);
	rename("temp", f_name);
	return l;//���������� �����������
}

int add_end(const char*f_name, Pair pp)
{
	fstream stream(f_name, ios::app);//������� ��� ����������
	if (!stream)
		return -1;//������ �������� �����

	stream << pp; //�������� ����� ������
	return 1;
}

int change_file(const char*f_name, int k, Pair pp)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)return -1;//������ �������� �����
	Pair p; int i = 0, l = 0;
	char x;
	while (stream >> p)
	{
		if (stream.eof())break;
		i++;
		if (i == k)
		{
			cout << p << " - is changing... Continue[y/n]?\n";
			cin >> x;
			if (x == 'n' || x == 'N')break;
			temp << pp;
			l++;
		}
		else temp << p;
	}
	stream.close(); temp.close();
	remove(f_name);
	rename("temp", f_name);
	return l;//���������� ���������� ���������
}

int delete_range(const char *f_name, Pair r1, Pair r2)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)
			return -1;//������ �������� �����

	char x;
	Pair p;
	int i = 0, deleted_lines = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		if (p < r1 && p > r2) {
			temp << p;
			continue;
		}
		deleted_lines++;
	}
	stream.close();
	temp.close();
	remove(f_name);
	rename("temp", f_name);
	return deleted_lines;//���������� ��������� ���������
}

int addtofile_begin(const char *f_name)
{
	fstream temp("temp", ios::out|ios::app);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)
		return -1;//������ �������� �����

	int count;
	cout << "Number of records: ";
	cin >> count;

	char x;
	Pair p;
	int i = 0;
	for (i = 0; i < count; i++) {
		cin >> p;
		temp << p;
	}

	while (stream >> p) {
		if (stream.eof())
			break;

		temp << p;
	}
	stream.close();
	temp.close();
	remove(f_name);
	rename("temp", f_name);
	return 0;
}

//// I
int delete_greather(const char *f_name, Pair val)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)
		return -1;//������ �������� �����

	Pair p;
	int i = 0, deleted_lines = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		if (p > val) {
			deleted_lines++;
			continue;
		}
		temp << p;
	}
	stream.close();
	temp.close();
	remove(f_name);
	rename("temp", f_name);
	return deleted_lines;//���������� ��������� ���������
}

int delete_greather(const char *f_name, Pair val)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)
		return -1;//������ �������� �����

	Pair p;
	int i = 0, deleted_lines = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		if (p > val) {
			deleted_lines++;
			continue;
		}
		temp << p;
	}
	stream.close();
	temp.close();
	remove(f_name);
	rename("temp", f_name);
	return deleted_lines;//���������� ��������� ���������
}

int increment_values(const char *f_name, Pair val)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)
		return -1;//������ �������� �����

	int c;
	cout << "Sub all values: ";
	cin >> c;

	Pair p;
	int i = 0, deleted_lines = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		if (p == val) {
			p.add(c);
			temp << p;
			continue;
		}
		temp << p;
	}
	stream.close();
	temp.close();
	remove(f_name);
	rename("temp", f_name);
	return deleted_lines;//���������� ��������� ���������
}

int addValsAt(const char *f_name, int k, int at)
{
	fstream temp("temp", ios::out);//������� ��� ������
	fstream stream(f_name, ios::in);//������� ��� ������
	if (!stream)
		return -1;//������ �������� �����

	int c;
	cout << "Sub all values: ";
	cin >> c;

	Pair p;
	int i = 0, deleted_lines = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		if (p == val) {
			p.add(c);
			temp << p;
			continue;
		}
		temp << p;
	}
	stream.close();
	temp.close();
	remove(f_name);
	rename("temp", f_name);
	return deleted_lines;//���������� ��������� ���������
}
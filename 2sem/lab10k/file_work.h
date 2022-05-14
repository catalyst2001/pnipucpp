#pragma once
#include "Time.h"
#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int make_file(const char* f_name)
{
	fstream stream(f_name, ios::out | ios::trunc);//открыть для записи
	if (!stream)
		return -1;//ошибка открытия файла

	int n;
	Time p;
	cout << "N?";
	cin >> n;
	for (int i = 0; i < n; i++)
	{
		cin >> p;//ввод атрибутов объекта из стандартного потока
		stream << p << "\n";//запись объекта в файловый поток
	}
	stream.close();//закрыть поток
	return n;//вернуть количество записанных объектов
}

int print_file(const char*f_name)
{
	fstream stream(f_name, ios::in);//открыть для чтения
	if (!stream)return -1;//ошибка открытия файла
	Time p; int i = 0;
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
	fstream temp("temp", ios::out);//открыть для записи
	fstream stream(f_name, ios::in);//открыть для чтения
	if (!stream)return -1;//ошибка открытия файла
	int i = 0; Time p;
	while (stream >> p)//пока нет конца файла выполняем чтение объекта
	{
		//если прочитан признак конца файла,то выход из цикла
		if (stream.eof())
			break;

		i++;
		//если номер объекта не равен k, то записываем его во вспомогательый файл
		if (i != k)
			temp << p;
	}
	//закрыть файлы
	stream.close();
	temp.close();
	remove(f_name);//удалить старый файл
	rename("temp", f_name);// переименовать temp
	return i;//количество прочитанных
}

int add_file(const char*f_name, int k, Time pp)
{
	fstream temp("temp", ios::out);//открыть для записи
	fstream stream(f_name, ios::in);//открыть для чтения
	if (!stream)return -1;//ошибка открытия файла
	Time p; int i = 0, l = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		i++;
		if (i == k)
		{
			temp << pp;//записать в temp новую запись
			l++;
		}
		temp << p;
	}
	stream.close(); temp.close();
	remove(f_name);
	rename("temp", f_name);
	return l;//количество добавленных
}

int add_end(const char*f_name, Time pp)
{
	fstream stream(f_name, ios::app);//открыть для добавления
	if (!stream)
		return -1;//ошибка открытия файла

	stream << pp; //записать новую запись
	return 1;
}

int change_file(const char*f_name, int k, Time pp)
{
	fstream temp("temp", ios::out);//открыть для записи
	fstream stream(f_name, ios::in);//открыть для чтения
	if (!stream)return -1;//ошибка открытия файла
	Time p; int i = 0, l = 0;
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
	return l;//количество измененных элементов
}

int delete_range(const char *f_name, Time r1, Time r2)
{
	fstream temp("temp", ios::out);//открыть для записи
	fstream stream(f_name, ios::in);//открыть для чтения
	if (!stream)
			return -1;//ошибка открытия файла

	char x;
	Time p;
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
	return deleted_lines;//количество удаленных элементов
}

int uvelitjit_vse_zapisi_s_znacheniem(const char *p_name)
{
	fstream temp("temp", ios::out);//открыть для записи
	fstream stream(p_name, ios::in);//открыть для чтения
	if (!stream)
		return -1;//ошибка открытия файла

	cout << "Znachenie dlya proverki:\n";
	Time znacjenie;
	cin >> znacjenie;

	char x;
	Time p;
	int i = 0;
	while (stream >> p)
	{
		if (stream.eof())
			break;

		if (p == znacjenie) {
			p.add_time();
			temp << p;
			i++;
		}
	}
	stream.close();
	temp.close();
	remove(p_name);
	rename("temp", p_name);
	return i;
}

int addtofile_begin(const char *f_name)
{
	fstream temp("temp", ios::out|ios::app);//открыть для записи
	fstream stream(f_name, ios::in);//открыть для чтения
	if (!stream)
		return -1;//ошибка открытия файла

	int count;
	cout << "Number of records: ";
	cin >> count;

	char x;
	Time p;
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
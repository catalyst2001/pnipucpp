#pragma once
#include <iostream>//���������� ��� ������ �� ������������ ��������
#include <fstream> //���������� ��� ������ �� ��������� ��������
#include <string>
using namespace std;

class Time
{
public:
	Time();
	Time(int m, int s);
	Time(const Time&);
	~Time();

	friend ostream& operator<<(ostream &out, const Time&p);
	friend istream& operator>>(istream &in, Time &p);

	Time operator=(const Time&);
	bool operator==(const Time &tm);
	bool operator!=(const Time &tm);

	bool operator>(const Time &tm);
	bool operator<(const Time &tm);

	void add_time();
private:
	int min;
	int sec;
};
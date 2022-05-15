#include "Time.h"

Time&Time::operator=(const Time&t)
{
	//�������� �� ����������������
	if (&t == this) return *this;
	min = t.min;
	sec = t.sec;
	return *this;
}
//���������� ���������� �������-�������� �����
istream&operator>>(istream&in, Time&t)
{
	cout << "min?"; in >> t.min;
	cout << "sec?"; in >> t.sec;
	return in;
}
//���������� ���������� �������-�������� ������
ostream&operator<<(ostream&out, const Time&t)
{
	return (out << t.min << " : " << t.sec);
}bool Time::operator <(const Time &t)
{
	if (min < t.min)return true;
	if (min == t.min&&sec < t.sec)return true;
	return false;
}
bool Time::operator >(const Time &t)
{
	if (min > t.min)return true;
	if (min == t.min&&sec > t.sec)return true;
	return false;
}
Time Time::operator+(const Time&t)
{
	int temp1 = min * 60 + sec;
	int temp2 = t.min * 60 + t.sec;
	Time p;
	p.min = (temp1 + temp2) / 60;
	p.sec = (temp1 + temp2) % 60;
	return p;
}
//���������� �������� �������� �������
Time Time::operator/(const Time&t)
{
	int temp1 = min * 60 + sec;
	int temp2 = t.min * 60 + t.sec;
	Time p;
	p.min = (temp1 / temp2) / 60;
	p.sec = (temp1 / temp2) % 60;
	return p;
}
Time Time::operator/(const int&t)
{
	int temp1 = min * 60 + sec;
	Time p;
	p.min = (temp1 / t) / 60;
	p.sec = (temp1 / t) % 60;
	return p;
}
#include "Time.h"

Time::Time()
{
	min = 0; sec = 0;
}

Time::Time(int m, int s)
{
	min = m; sec = s;
}

Time::Time(const Time&p)
{
	min = p.min; sec = p.sec;
}

Time Time::operator =(const Time&p)
{
	if (&p == this)
		return *this;

	min = p.min; sec = p.sec;
	return*this;
}

bool Time::operator==(const Time &tm)
{
	return min == tm.min && sec == tm.sec;
}

bool Time::operator!=(const Time & tm)
{
	return min != tm.min && sec != tm.sec;
}

bool Time::operator>(const Time &tm)
{
	return min > tm.min && sec > tm.sec;
}

bool Time::operator<(const Time & tm)
{
	return min < tm.min && sec < tm.sec;
}

void Time::add_time()
{
	min++;
	sec += 30;
	if (sec > 59) {
		min++;
		sec = sec % 60;
	}
}

Time::~Time()
{
}

ostream& operator<<(ostream& out, const Time &p)
{
	out << p.min << ":" << p.sec << "\n";
	return out;
}

istream& operator>>(istream& in, Time&p)
{
	cout << "min?"; in >> p.min;
	cout << "sec?"; in >> p.sec;
	if (p.sec > 59)
		p.sec = 59;

	return in;
}
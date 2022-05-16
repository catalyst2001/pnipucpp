#include "Time.h"

Time::Time(int seconds)
{
	min = seconds / 60;
	sec = seconds % 60;
}

void Time::set_sec(int s)
{
	if (s > 60)
		s = 60;

	sec = s;
}

int Time::to_seconds()
{
	return min * 60 + sec;
}

void Time::print()
{
	std::cout << min << ":" << sec << std::endl;
}

bool Time::operator==(const Time &t)
{
	return (t.min == min && t.sec == sec);
}

bool Time::operator!=(const Time &t)
{
	return (t.min != min && t.sec != sec);
}

bool Time::operator<(const Time & t)
{
	return (min * 60 + sec) < (t.min * 60 + t.sec);
}

bool Time::operator>(const Time & t)
{
	return (min * 60 + sec) > (t.min * 60 + t.sec);
}

const Time Time::operator/(const Time & t)
{
	int tempT = (t.min * 60 + t.sec);
	int totalSec = (min * 60 + sec) / (tempT == 0 ? 1 : tempT);
	int min = totalSec / 60;
	return Time(min, totalSec - (60 * min));
}

const Time Time::operator/(int i)
{
	int totalSec = (min * 60 + sec) / i;
	int min = totalSec / 60;
	return Time(min, totalSec - (60 * min));
}

const Time Time::operator+(const Time & t)
{
	int totalSec = (min * 60 + sec) + (t.min * 60 + t.sec);
	int min = totalSec / 60;
	return Time(min, totalSec - (60 * min));
}

std::istream &operator>>(std::istream &in, Time &t)
{
	std::cout << "enter min: ";
	std::cin >> t.min;
	std::cout << "enter sec: ";
	std::cin >> t.sec;
	return in;
}

std::ostream &operator<<(std::ostream &out, const Time &t)
{
	return out << t.min << ":" << t.sec << std::endl;
}
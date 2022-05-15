#include "myTime.h"

void myTime::set_sec(int s)
{
	if (s > 60)
		s = 60;

	sec = s;
}

void myTime::print()
{
	std::cout << min << ":" << sec << std::endl;
}

bool myTime::operator==(const myTime &t)
{
	return (t.min == min && t.sec == sec);
}

bool myTime::operator!=(const myTime &t)
{
	return (t.min != min && t.sec != sec);
}

bool myTime::operator<(const myTime & t)
{
	return (min * 60 + sec) < (t.min * 60 + t.sec);
}

bool myTime::operator>(const myTime & t)
{
	return (min * 60 + sec) > (t.min * 60 + t.sec);
}

const myTime& myTime::operator/(const myTime & t)
{
	int tempT = (t.min * 60 + t.sec);
	int totalSec = (min * 60 + sec) / (tempT == 0 ? 1 : tempT);
	int min = totalSec / 60;
	return myTime(min, totalSec - (60 * min));
}

const myTime& myTime::operator/(int i)
{
	int totalSec = (min * 60 + sec) / i;
	int min = totalSec / 60;
	return myTime(min, totalSec - (60 * min));
}

const myTime& myTime::operator+(const myTime & t)
{
	int totalSec = (min * 60 + sec) + (t.min * 60 + t.sec);
	int min = totalSec / 60;
	return myTime(min, totalSec - (60 * min));
}

std::istream &operator>>(std::istream &in, myTime &t)
{
	std::cout << "enter min: ";
	std::cin >> t.min;
	std::cout << "enter sec: ";
	std::cin >> t.sec;
	return in;
}

std::ostream &operator<<(std::ostream &out, const myTime &t)
{
	return out << t.min << ":" << t.sec << std::endl;
}

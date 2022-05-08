#include "time.h"
#include "time.h"

void time::set_sec(int s)
{
	if (s > 60)
		s = 60;

	sec = s;
}

void time::print()
{
	std::cout << min << ":" << sec << std::endl;
}

bool time::operator==(const time &t)
{
	return (t.min == min && t.sec == sec);
}

bool time::operator!=(const time &t)
{
	return (t.min != min && t.sec != sec);
}

std::istream &operator>>(std::istream &in, time &t)
{
	std::cout << "enter min: ";
	std::cin >> t.min;
	std::cout << "enter sec: ";
	std::cin >> t.sec;
	return in;
}

std::ostream &operator<<(std::ostream &out, const time &t)
{
	return out << t.min << ":" << t.sec << std::endl;
}

#include "kvitanciya.h"

kvitanciya::kvitanciya(int numb, std::string dat, double dsum)
{
	number = numb;
	date = dat;
	sum = dsum;
	std::cout << "Called contructor with parameters for object " << this << "\n";
}

kvitanciya::kvitanciya(kvitanciya &ref)
{
	*this = ref;
	std::cout << "Called copy contructor for object " << this << "\n";
}

void kvitanciya::setNumber(int n)
{
	number = n;
}

int kvitanciya::getNumber()
{
	return number;
}

void kvitanciya::setDate(std::string strdate)
{
	date = strdate;
}

std::string kvitanciya::getDate()
{
	return date;
}

void kvitanciya::setSum(double s)
{
	sum = s;
}

double kvitanciya::getSum()
{
	return sum;
}

void kvitanciya::show()
{
	std::cout << "Number: " << number << " Sum: " << sum << " Date: " << date.c_str() << "\n";
}
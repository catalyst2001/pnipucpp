#include "Pair.h"

Pair::Pair()
{
	first = 0; next = 0;
}

Pair::Pair(int m, int s)
{
	first = m; next = s;
}

Pair::Pair(const Pair&p)
{
	first = p.first; next = p.next;
}

Pair Pair::operator =(const Pair&p)
{
	if (&p == this)
		return *this;

	first = p.first; next = p.next;
	return*this;
}

bool Pair::operator==(const Pair &tm)
{
	return first == tm.first && next == tm.next;
}

bool Pair::operator!=(const Pair & tm)
{
	return first != tm.first && next != tm.next;
}

void Pair::operator++()
{
	first++;
}

void Pair::operator++(int)
{
	next++;
}

bool Pair::operator>(const Pair &tm)
{
	return first > tm.first && next > tm.next;
}

bool Pair::operator<(const Pair & tm)
{
	return first < tm.first && next < tm.next;
}

void Pair::add(int c)
{
	first += c;
	next += c;
}

Pair::~Pair()
{
}

ostream& operator<<(ostream& out, const Pair &p)
{
	out << p.first << ":" << p.next << "\n";
	return out;
}

istream& operator>>(istream& in, Pair&p)
{
	cout << "first?"; in >> p.first;
	cout << "next?"; in >> p.next;
	if (p.next > 59)
		p.next = 59;

	return in;
}
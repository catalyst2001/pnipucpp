#include "Emloyee.h"

Emloyee::Emloyee(void) : Person()
{
	doljnost = "";
	zarplata = 0.f;
}

Emloyee::~Emloyee(void)
{
}

Emloyee::Emloyee(const Emloyee &L)
{
	name = L.name;
	age = L.age;
	doljnost = L.doljnost;
	zarplata = L.zarplata;
}

Emloyee& Emloyee::operator=(const Emloyee&L)
{
	if (&L == this)
		return *this;

	name = L.name;
	age = L.age;
	doljnost = L.doljnost;
	zarplata = L.zarplata;
	return *this;
}

void Emloyee::Show()
{
	cout << "\nname : " << name;
	cout << "\nage : " << age;
	cout << "\ndoljnost : " << doljnost;
	cout << "\nzarplata : " << zarplata;
	cout << "\n";
}

void Emloyee::Input()
{
	cout << "\nname : " << name;  cin >> name;
	cout << "\nage : " << age; cin >> age;
	cout << "\ndoljnost : " << doljnost; cin >> doljnost;
	cout << "\nzarplata : " << zarplata; cin >> zarplata;
}

Emloyee::Emloyee(string nname, int aage, string ddoljnost, float zzarplata)
{
	name = nname;
	age = aage;
	doljnost = ddoljnost;
	zarplata = zzarplata;
}

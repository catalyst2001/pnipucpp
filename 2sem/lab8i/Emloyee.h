#pragma once
#include "Person.h"

class Emloyee : public Person
{
public:
	Emloyee();
	~Emloyee();
	Emloyee(string name, int age, string doljnost, float zarplata);
	Emloyee(const Emloyee &);
	float get_zarplata() { return zarplata; }
	std::string get_doljnost() { return doljnost; }
	Emloyee& operator=(const Emloyee&);

	void Show();
	void Input();
protected:
	float zarplata;
	std::string doljnost;
};


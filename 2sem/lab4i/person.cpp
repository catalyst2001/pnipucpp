#include "person.h"

void person::setName(std::string n)
{
	name = n;
}

std::string person::getName() const
{
	return name;
}

void person::setAge(int a)
{
	age = a;
}

int person::getAge() const
{
	return age;
}

std::istream &operator>>(std::istream &in, person &t)
{
	std::cout << "person name: ";
	std::cin >> t.name;
	std::cout << "person age: ";
	std::cin >> t.age;
	return in;
}

std::ostream &operator<<(std::ostream &out, const person &t)
{
	return out << "Person print:  Name: " << t.name << "   Age: " << t.age << std::endl;
}
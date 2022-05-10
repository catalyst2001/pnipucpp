#include "student.h"

std::istream &operator>>(std::istream &in, student &p)
{
	int age;
	std::string name;
	std::cout << "student name: ";
	std::cin >> name;
	p.setName(name);
	std::cout << "student age: ";
	std::cin >> age;
	p.setAge(age);
	std::cout << "learning year: ";
	std::cin >> p.yearLearning;
	return in;
}

std::ostream &operator<<(std::ostream &out, const student &p)
{
	return out << p.getName() << " " << p.getAge() << " " << p.getLearnYear() << std::endl;
}

void student::operator=(const student &p)
{
	setAge(p.getAge());
	setName(p.getName());
	setLearnYear(p.getLearnYear());
	std::cout << __FUNCSIG__ " for object " << this << " called\n";
}

person student::getPerson()
{
	std::cout << __FUNCSIG__ " for object " << this << " called\n";
	return *this;
}
#include "student.h"

std::istream &operator>>(std::istream &in, student &p)
{
	std::cout << "learning year: ";
	std::cin >> p.yearLearning;
	return in;
}

std::ostream &operator<<(std::ostream &out, const student &p)
{
	return out << "student: " << p.getLearnYear() << std::endl;
}

void student::operator=(const student &p)
{
	setLearnYear(p.getLearnYear());
}

void student::show()
{
	std::cout << "void student::show(): " << getLearnYear() << "\n";
}
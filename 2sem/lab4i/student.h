#pragma once
#include "person.h"

class student : person
{
	int yearLearning;
public:
	student() {
		std::cout << __FUNCSIG__ " constructor without parameters for object " << this << " called\n";
	}
	student(int _age, std::string _name, int year) {
		setAge(_age);
		setName(_name);
		yearLearning = year;
		std::cout << __FUNCSIG__ " constructor with parameters for object " << this << " called\n";
	}
	student(const student &s) {
		*this = s;
		std::cout << __FUNCSIG__ " constructor copy for object " << this << " called\n";
	}
	~student() {
		std::cout << __FUNCSIG__ " destructor for object " << this << " called\n";
	}
	void setLearnYear(int y) { yearLearning = y; }
	int getLearnYear() const { return yearLearning; }
	void incLearnYear() { yearLearning++; }
	friend std::istream &operator>>(std::istream &in, student &p);
	friend std::ostream &operator<<(std::ostream &out, const student &p);
	void operator=(const student &p);
	person getPerson();
};
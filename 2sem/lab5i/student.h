#pragma once
#include "person.h"

class student : public base
{
	int yearLearning;
public:
	student() {}
	student(int year) {
		yearLearning = year;
	}
	student(const student &s) { *this = s; }
	~student() {}
	void setLearnYear(int y) { yearLearning = y; }
	int getLearnYear() const { return yearLearning; }
	void incLearnYear() { yearLearning++; }
	friend std::istream &operator>>(std::istream &in, student &p);
	friend std::ostream &operator<<(std::ostream &out, const student &p);
	void operator=(const student &p);
	void show();
};
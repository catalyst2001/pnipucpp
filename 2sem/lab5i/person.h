#pragma once
#include <iostream>
#include <string>
#include "base.h"

class person : public base
{
	std::string name;
	int age;
public:
	person() {
		name = std::string("Unnamed");
		age = 0;
	}
	person(std::string n, int a) {
		name = n;
		age = a;
	}
	person(const person &p) { *this = p; }
	~person() { }

	void setName(std::string n);
	std::string getName() const;
	void setAge(int a);
	int getAge() const;

	void show();

	friend std::istream &operator>>(std::istream &in, person &p);
	friend std::ostream &operator<<(std::ostream &out, const person &p);
};


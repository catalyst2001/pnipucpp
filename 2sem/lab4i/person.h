#pragma once
#include <iostream>
#include <string>

class person
{
	std::string name;
	int age;
public:
	person() {
		name = std::string("Unnamed");
		age = 0;
		std::cout << __FUNCSIG__ " for object " << this << " called\n";
	}
	person(std::string n, int a) {
		name = n;
		age = a;
		std::cout << __FUNCSIG__ " for object " << this << " called\n";
	}
	person(const person &p) {
		*this = p;
		std::cout << __FUNCSIG__ " for object " << this << " called\n";
	}
	~person() {
		std::cout << __FUNCSIG__ " for object " << this << " called\n";
	}

	void setName(std::string n);
	std::string getName() const;
	void setAge(int a);
	int getAge() const;

	friend std::istream &operator>>(std::istream &in, person &p);
	friend std::ostream &operator<<(std::ostream &out, const person &p);
};


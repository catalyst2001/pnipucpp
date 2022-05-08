#pragma once
#include <iostream>

class kvitanciya
{
	int number;
	std::string date;
	double sum;
public:
	kvitanciya() {
		number = 0;
		date = "<null>";
		sum = 0.0;
		std::cout << "Called default contructor for object " << this << "\n";
	};
	kvitanciya(int numb, std::string dat, double dsum);
	kvitanciya(kvitanciya &ref);
	~kvitanciya() {
		std::cout << "Called destructor for object " << this << "\n";
	}

	void setNumber(int n);
	int getNumber();
	void setDate(std::string strdate);
	std::string getDate();
	void setSum(double s);
	double getSum();
	void show();
};

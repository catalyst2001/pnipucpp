#pragma once
#include <iostream>

struct fraction
{
	double first, second;
	void init(double f, double s);
	void read();
	void show();

	bool range_check(double x);
};


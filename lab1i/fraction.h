#pragma once
#include <iostream>

struct fraction
{
	int first;
	double second;
	void init(int f, double s);
	void read();
	void show();

	double cost();
};

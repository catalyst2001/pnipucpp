#pragma once
#include "Print.h"

class Book : public Print
{
public:
	Book();
	~Book();

	void Show();
	void Input();

	Book(string izd, string name, string auth);
	Book(const Book &);
	int get_stranic() { return stranic; }
	std::string get_izdatelstvo() { return izdatelstvo; }
	Book& operator=(const Book&);

protected:
	int stranic;
	std::string izdatelstvo;
};


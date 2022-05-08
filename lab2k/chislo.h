#pragma once
#include <iostream>
#include <string>

class chislo
{
	float mantissa;
	int poryadok;
	std::string strimage;
public:
	chislo() : poryadok(0), mantissa(0.f), strimage("0.0") {
		std::cout << "Constructor for object " << this << " without parameters was called" << std::endl;
	};
	chislo(int p, float m, std::string strimg) : poryadok(p), mantissa(m), strimage(strimg) {
		std::cout << "Constructor for object " << this << " with parameters was called" << std::endl;
	};
	chislo(chislo &ref);
	~chislo() {
		std::cout << "Destructor for object " << this << " was called" << std::endl;
	};

	int get_poryadok();
	void set_poryadok(int p);
	float get_mantissa();
	void set_mantissa(float m);
	std::string get_strimage();
	void set_strimage(std::string strimg);
	void string_image_from_fields();
	std::string get_string_image();
	void show();
};


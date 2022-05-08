#include "chislo.h"

chislo::chislo(chislo &ref)
{
	std::cout << "Constructor copy was called" << std::endl;
	*this = ref;
}

int chislo::get_poryadok()
{
	return poryadok;
}

void chislo::set_poryadok(int p)
{
	poryadok = p;
}

float chislo::get_mantissa()
{
	return mantissa;
}

void chislo::set_mantissa(float m)
{
	mantissa = m;
}

std::string chislo::get_strimage()
{
	return strimage;
}

void chislo::set_strimage(std::string strimg)
{
	strimage = strimg;
}

void chislo::string_image_from_fields()
{
	strimage = std::to_string(poryadok) + "." + std::to_string((int)mantissa);
}

std::string chislo::get_string_image()
{
	string_image_from_fields();
	return strimage;
}

void chislo::show()
{
	string_image_from_fields();
	std::cout << get_string_image() << std::endl;
}

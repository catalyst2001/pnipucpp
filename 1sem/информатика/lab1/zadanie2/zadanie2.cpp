// zadanie2.cpp
#include <iostream>

int main()
{
	int inn, inm, n, m, result;
	std::cin >> inn >> inm;
	n = inn;
	m = inm;
	result = n++ * m;
	std::cout << "1) " << result << std::endl;

	n = inn;
	m = inm;
	result = n++ < m;
	std::cout << "2) " << result << std::endl;

	m = inm;
	result = m-- > m;
	std::cout << "3) " << result << std::endl;
	return 0;
}

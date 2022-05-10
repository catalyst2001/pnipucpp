// lab2.cpp
#include <iostream>

int main()
{
	setlocale(LC_ALL, "ru");
	int j = 0;
	for (int i = 21; i < 100; i++)
		if (!(i % 3))
			j += i;

	std::cout << "Сумма целых положительных чисел, больших 20, меньших 100 и кратных 3  равна: " << j << std::endl;
	return 0;
}
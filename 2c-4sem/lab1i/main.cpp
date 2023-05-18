#include <iostream>

#define TRACE_MESSAGES 1

#define RECURSIVE

#ifdef RECURSIVE
void nearestNoMultipleNumber(int &num)
{
#if TRACE_MESSAGES == 1
	static int previousValue = num;
	std::cout << "first=" << previousValue  << " current=" << num << std::endl;
#endif

	if (!(num % 2))
	{
		num--;
		nearestNoMultipleNumber(num);
	}
}

int main()
{
	const int n = 14;
	int i = n;
	nearestNoMultipleNumber(i);
	std::cout << "n=" << n << " uneven=" << i << std::endl;
	return 0;
}

#else

int main()
{
	const int n = 14;
	int i = n;
	while (!(i % 2)) {
#if TRACE_MESSAGES == 1
		std::cout << "first=" << n << " current=" << i << std::endl;
#endif
		i--;
	}

	std::cout << "n=" << n << " uneven=" << i << std::endl;
	return 0;
}

#endif
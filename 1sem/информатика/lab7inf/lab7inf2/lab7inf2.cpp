#include <iostream>

#define EPSILON 0.0001
#define RANGE 99999
#define LEFT_RANGE 0.0
#define RIGHT_RANGE 1.5

#define MyFunc(x) (pow(x, 5) - cos(pow(x, 2)))
#define MyFunc2(x) (pow(x, 5) - (5.0 * pow(x, 4)) + (10.0 * pow(x, 3)) - (10.0 * (x*x)) + (5.0 * x) - 1.0 - (cos((x*x) - 2.0 * x + 1.0)))


int main()
{
	float x0 = LEFT_RANGE, x1 = RIGHT_RANGE;
	for (int i = 0; i < RANGE; i++) {
		x1 = MyFunc2(x0);

		float cc = fabs(x1 - x0);
		//std::cout << "cc = " << cc << std::endl;
		if (cc > EPSILON)
			break;

		x0 = x1;
	}
	std::cout << "X = " << fabs(x1) << std::endl;
	return 0;
}
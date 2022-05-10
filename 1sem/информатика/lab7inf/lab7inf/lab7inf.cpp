#include <iostream>

#define EPSILON 0.0001
#define RANGE 9999

#define LEFT_RANGE 0.0
#define RIGHT_RANGE 1.5

#define MyFunc(x) (pow(x, 5) - cos(pow(x, 2)))

int main()
{
	double a = LEFT_RANGE;
	double b = RIGHT_RANGE;
	double tempa = MyFunc(a);
	double tempb = MyFunc(b);

	int i = 0;
	double midx, funcmidx;
	while (((b - a) > EPSILON) && (i < RANGE)) {
		midx = (a + b) / 2;
		funcmidx = MyFunc(midx);
		if ((tempa * funcmidx) <= 0) {
			b = midx;
			tempb = funcmidx;
		}
		else {
			a = midx;
			tempa = funcmidx;
		}
		i++;
	}
	std::cout << "X = " << (a + b) / 2 << std::endl;
	return 0;
}
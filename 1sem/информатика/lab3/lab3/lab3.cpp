#include <iostream>
#include <math.h>

#define EPSILON 0.0001

int main()
{
	int n = 10;
	int k = 10;

	float a = 0.1f;
	float b = 0.8f;
	float h = (b - a) / k;
	float an, p, prev, SN, SE, Y;
	for (float x = a; x <= b + h; x += h) {
		p = x;
		SN = 0.f;

		int i;
		for (i = 1; i <= n; i++) {
			float tmp = 2.f * i;
			p *= pow(-1.f, i + 1.f) * ((pow(x, tmp)) / (tmp*(tmp - 1.f)));
			SN += p;
		}

		p = x;
		an = 0.f;
		prev = 1.f;
		SE = 0.f;
		for (i = 1; fabs(prev - an) > EPSILON; i++) {
			p *= x;
			prev = an;
			float tmp = 2.f * i;
			an = p * pow(-1.f, i + 1.f) * (pow(x, tmp) / (tmp * (tmp - 1.f)));
			SE += an;
		}
		Y = x * atan(x) - log(sqrt(1.f + x * x));
		std::cout << "X=" << x << "\tSN=" << SN << "\tSE=" << SE << "\tY=" << Y << std::endl;
	}
	return 0;
}
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

long long sum(int nparams, ...)
{
	long long S = 0;
	int *pparam = (&nparams) + 1;
	for (int i = 0; i < nparams; i += 2)
		S += pparam[i] * pparam[i + 1];

	return S;
}

void gen_parameters(int *ptr)
{
	srand(clock());
	for (int i = 0; i < 12; i++)
		ptr[i] = rand() % 12;
}

int main()
{
	int params[12];
	for (int i = 0; i < 3; i++) {
		gen_parameters(params);
		printf("%d: Parameters 8, 10, 12: ( ( ( %d %d %d %d %d %d %d %d ) %d %d ) %d %d )\n", i + 1,
			params[0], params[1], params[2], params[3], params[4], params[5],
			params[6], params[7], params[8], params[9], params[10], params[11]);

		printf("%d: sum 8 parameters: %lld\n", i + 1,
			sum(8, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7]));

		printf("%d: sum 10 parameters: %lld\n", i + 1,
			sum(10, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9]));

		printf("%d: sum 12 parameters: %lld\n", i + 1,
			sum(12, params[0], params[1], params[2], params[3], params[4], params[5], params[6], params[7], params[8], params[9], params[10], params[11]));

		printf("\n");
	}
	return 0;
}
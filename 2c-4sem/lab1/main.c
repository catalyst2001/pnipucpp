#include <stdio.h>
#include <stdlib.h>

#ifdef min
#undef min
#endif

#define TRACE_MSG_ENABLE

#ifdef TRACE_MSG_ENABLE
#define TRACEMSG(x, ...) printf(x "\n", __VA_ARGS__)
#else
#define TRACEMSG(x, ...)
#endif

#define CNT(x) (sizeof(x) / sizeof(x[0]))

void fill_rand(int *p_dst, int count)
{
	printf("array: ");
	srand((unsigned int)time(0));
	for (int i = 0; i < count; i++) {
		p_dst[i] = rand() % 100;
		printf("%d ", p_dst[i]);
	}
	putc('\n', stdout);
}

//#define RECURSIVE
#ifndef RECURSIVE

int main()
{
	int arr[10];
	int min, i;

	/* fill by random numbers of 0 to 100 */
	fill_rand(arr, CNT(arr));

	/* find min elem */
	min = arr[0];
	for (i = 0; i < CNT(arr); i++) {
		if (arr[i] < min) 
			min = arr[i];

		TRACEMSG("i=%d  array[%d]=%d  min=%d", i, i, arr[i], min);
	}

	printf("min element in array: %d\n", min);
	return 0;
}

#else

int find_min(const int *p_src, int count, int currmin)
{
	if (!count--)
		return currmin;

	if (p_src[count] < currmin)
		currmin = p_src[count];

	TRACEMSG("find_min: remaining=%d currmin=%d", count, currmin);
	return find_min(p_src, count, currmin);
}

int main()
{
	int arr[10];

	fill_rand(arr, CNT(arr));
	printf("min value in array: %d\n", find_min(arr, CNT(arr), arr[0]));
	return 0;
}

#endif
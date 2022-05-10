// lab6.cpp
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>
#include <ctype.h> //isdigit

const int max_strings = 8;

bool digit_contain(const char *str)
{
	for (int i = 0; i < strlen(str); i++)
		if (isdigit(str[i]))
			return true;

	return false;
}

int main()
{
	char buffers[max_strings][32];

	int i;
	for (i = 0; i < max_strings; i++) {
		printf("Enter string ( %d / %d )\n", i + 1, max_strings);
		gets_s(buffers[i], sizeof(buffers) / max_strings);
	}

	printf("\nStrings contain numbers:\n");
	for (i = 0; i < max_strings; i++)
		if (digit_contain(buffers[i]))
			printf("%d: %s\n", i + 1, buffers[i]);
	return 0;
}
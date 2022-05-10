#include <iostream>

#define STRING_LENGTH 32

bool fill_string(char strings[][STRING_LENGTH], int num_of_strings, int index)
{
	if (index < 0 || index >= num_of_strings)
		return false;

	for (int i = 0; i < STRING_LENGTH-1; i++)
		strings[index][i] = '0';

	return true;
}

bool delete_string(char strings[][STRING_LENGTH], int num_of_strings, const char *string)
{
	auto find_string = [strings, num_of_strings](const char *string) -> int {
		for (int i = 0; i < num_of_strings; i++)
			if (!strcmp(strings[i], string))
				return i;
		
		return -1;
	};

	int index;
	if ((index = find_string(string)) == -1) {
		std::cout << "String not found!" << std::endl;
		return false;
	}
	return fill_string(strings, num_of_strings, index);
}

void print_strings(char strs[][STRING_LENGTH], int number_of_strings)
{
	for (int i = 0; i < number_of_strings; i++)
		std::cout << strs[i] << std::endl;
}

int main()
{
	char strs[][STRING_LENGTH] = {
		"string0",
		"string1",
		"string2",
		"string3",
		"string4",
		"string5",
		"string6",
		"string7",
		"string8",
		"string9"
	};

	std::cout << "Original strings array:\n";
	const int number_of_strings = sizeof(strs) / STRING_LENGTH;
	print_strings(strs, number_of_strings);

	std::cout << "\n\nDeleting string 'string5':\n";
	if (!delete_string(strs, number_of_strings, "string5")) {
		std::cout << "failed delete string!" << std::endl;
		return 1;
	}
	print_strings(strs, number_of_strings);

	std::cout << "\n\nDeleting strings 1-5:\n";
	int A = 1;
	int B = 5;
	for (; A < B; A++)
		fill_string(strs, number_of_strings, A);

	print_strings(strs, number_of_strings);
	return 0;
}
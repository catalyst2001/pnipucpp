// lab4.cpp
#include <iostream>
#include <time.h>

//#define KIRILL

int random()
{
	return (rand() % 7) ? rand() : -rand();
}

#ifdef KIRILL
int find_element_in_array(int *parr, int newsize, int element)
{
	for (int i = 0; i < newsize; i++)
		if (parr[i] == element)
			return i;

	return -1;
}

bool remove_element_from_array(int *parr, int *pnewsize, int size, int element)
{
	int ind = find_element_in_array(parr, *pnewsize, element);
	if (ind == -1)
		return false;

	
	for (int i = ind; i < size - 1; i++)
		parr[i] = parr[i + 1];

	(*pnewsize)--;
	return true;
}

void print_array(int *p, int size)
{
	for (int i = 0; i < size; i++)
		std::cout << p[i] << " ";

	std::cout << std::endl;
}

int main()
{
	srand((unsigned int)time(NULL));

	int i;
	int array[10];
	const int array_size = sizeof(array) / sizeof(array[0]);
	int newsize = array_size;

	//Сформировать одномерный массив целых чисел, используя датчик случайных чисел
	for (i = 0; i < array_size; i++)
		array[i] = random();

	//Распечатать полученный массив
	print_array(array, newsize);

	//Удалить элементы, индексы которых кратны 3
	for (i = 0; i < array_size; i++)
		if (!(array[i] % 3))
			remove_element_from_array(array, &newsize, array_size, array[i]);

	//Добавить после каждого отрицательного элемента массива элемент со значением  |M[i - 1] + 1|
#define abs(n) ((n < 0) ? -n : n)
	for (i = 0; i < newsize - 1; i++)
		if (array[i] < 0)
			array[i + 1] = abs(array[i - 1] + 1);
#undef abs

	print_array(array, newsize);
	return 0;
}

#else

//IGOR

#define arraySize 20
using namespace std;

int main()
{
	int arr[arraySize];
	for (int i = 0; i < arraySize; i++)
		arr[i] = random();

	for (int i = 0; i < arraySize; i++)
		cout << arr[i] << " ";
	cout << "\n";

	int element;
	cout << "Enter value for delete: ";
	cin >> element;

	int size = arraySize;
	for (int i = 0; i < arraySize; i++) {
		if (arr[i] == element) {
			size--;
			for (int j = i; j < size; j++)
				arr[j] = arr[j + 1];

			break;
		}
	}

	for (int i = 0; i < arraySize; i++)
		cout << arr[i] << " ";
	cout << "\n";

	int shiftCount;
	int firstElement;
	cout << "Enter shift count: ";
	cin >> shiftCount;
	for (int i = 0; i < shiftCount; i++) {
		firstElement = arr[arraySize - 1];
		for (int j = arraySize - 1; j > 0; j--)
			arr[j] = arr[j - 1];

		arr[0] = firstElement;
	}

	for (int i = 0; i < arraySize; i++)
		cout << arr[i] << " ";
	cout << "\n";
	return 0;
}

#endif
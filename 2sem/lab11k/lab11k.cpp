#include <iostream>
#include <deque>
#include <intrin.h>
#include <limits>
using namespace std;

void print_deque(deque<int> &dqueueref)
{
	for (auto c : dqueueref)
		cout << c << endl;
}

int main()
{
	const int count = 10;

	//random fill
	size_t i;
	deque<int> dqueue;
	for (i = 0; i < count; i++) {
		srand((unsigned int)__rdtsc());
		dqueue.push_back((rand() % 100) - 10);
	}
	print_deque(dqueue);

	// average
	cout << "\nfind avg:\n";
	int average = 0;
	for (i = 0; i < dqueue.size(); i++)
		average += dqueue[i];

	average /= dqueue.size();
	cout << "Avg: " << average << "\n";

	//delete maximum
	struct { int m, i; } maxd = { INT_MIN, 0 };
	for (i = 0; i < dqueue.size(); i++) {
		if (maxd.m < dqueue[i]) {
			maxd.m = dqueue[i];
			maxd.i = i;
		}
	}

	//delete maximum
	cout << "\ndelete maximum:\n";
	dqueue.erase(dqueue.begin() + maxd.i);
	print_deque(dqueue);

	// find min
	int min = INT_MAX;
	for (i = 0; i < dqueue.size(); i++)
		if (dqueue[i] < min)
			min = dqueue[i];

	//division by min
	cout << "\ndivision by min:\n";
	for (i = 0; i < dqueue.size(); i++)
		dqueue[i] /= min;

	print_deque(dqueue);
	return 0;
}
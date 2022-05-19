// lab11i4.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <deque>
#include "../lab3i/pair.h"

template<typename T>
class QueueConcurency
{
	std::deque<T> dqueue;
public:
	QueueConcurency() {}
	~QueueConcurency() {}
	QueueConcurency(int size) { dqueue.resize(size); }

	QueueConcurency &operator=(const QueueConcurency &from) const {
		size_t sz = from.dqueue.size();
		dqueue.resize(sz);
		for (size_t i = 0; i < sz; i++)
			dqueue[i] = from.dqueue[i];

		return *this;
	}

	T &begin() const { return dqueue[0]; }
	T &end() const { return dqueue[dqueue.size() - 1]; }

	T &operator[](size_t idx) { return dqueue[idx]; }
	size_t operator()() const { return dqueue.size(); }
	size_t size() const { return dqueue.size(); }

	void insert_at(int idx, T elem) { dqueue.insert(dqueue.begin() + idx, elem); }
	void delete_at(int idx) { dqueue.erase(dqueue.begin() + idx); }

	void push(T elem) { dqueue.push_back(elem); }
	void push_back(T elem) { dqueue.push_back(elem); }
	void push_back_by_ref(T &elem) { dqueue.push_back(elem); }
};

void print(QueueConcurency<pair> &s)
{
	for (size_t i = 0; i < s(); i++)
		std::cout << s[i] << std::endl;
}

void fill_rand(QueueConcurency<pair> &s)
{
	for (int i = 0; i < 10; i++) {
		srand((unsigned int)__rdtsc());
		s.push_back(pair(i, rand() % 60));
	}
}

int compute_avg(QueueConcurency<pair> &s)
{
	double avg = 0.;
	for (size_t i = 0; i < s(); i++)
		avg += s[i].getValue();

	avg /= s();
	return avg;
}

void delete_elements_by_keyrange(QueueConcurency<pair> &p, int keybegin, int keyend)
{
	int key;
	for (size_t i = 0; i < p.size(); i++) {
		key = p[i].getKey();
		if (key > keybegin && key < keyend) {
			p.delete_at(i);
		}
	}
}

int random(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

int main()
{
	QueueConcurency<pair> qc;
	fill_rand(qc);
	print(qc);

	int kmin = random(0, qc.size());
	int kmax = random(kmin + 1, qc.size());
	std::cout << "key_event min: " << kmin << "   key_event max: " << kmax << "\n";
	delete_elements_by_keyrange(qc, kmin, kmax);
	print(qc);
	return 0;
}
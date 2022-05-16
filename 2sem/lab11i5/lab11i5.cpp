// lab11i5.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include <deque>
#include <list>

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

int &get_elem_from_list(std::list<int> &l, int idx)
{
	std::list<int>::iterator it = l.begin();
	for (int i = 0; i < idx; i++)
		it++;

	return *it;
}

int compute_avg_container(std::list<int> &s)
{
	double avg = 0.;
	for (size_t i = 0; i < s.size(); i++)
		avg += get_elem_from_list(s, i);

	avg /= s.size();
	return avg;
}

void print(QueueConcurency<std::list<int>> &q)
{
	for (size_t i = 0; i < q.size(); i++) {
		std::cout << "Queue idx: " << i << " List: ";
		for (size_t j = 0; j < q[i].size(); j++) {
			std::cout << get_elem_from_list(q[i], j) << " ";
		}
		std::cout << std::endl;
	}
}

int main()
{
	QueueConcurency<std::list<int>> queue(10);
	for (size_t i = 0; i < queue.size(); i++)
		for (size_t j = 0; j < 10; j++)
			queue[i].push_back(i + j);

	print(queue);
	
	for (size_t i = 0; i < queue.size(); i++) {
		int avg = compute_avg_container(queue[i]);
		for (size_t j = 0; j < queue[i].size(); j++)
			get_elem_from_list(queue[i], j) -= avg;
	}

	std::cout << "\n\n";
	print(queue);
	return 0;
}
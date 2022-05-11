#pragma once
#include <stdlib.h>
#include <iostream>

void *operator new (size_t size);
void operator delete(void *ptr);

#define assert(x) if(!(x)) __asm int 3h;

struct node {
	int number;
	node *p_lastnode;
	node *p_nextnode;
};

class listiter
{
	friend class list;
	node *ptr;
public:
	listiter() : ptr(nullptr) {}
	~listiter() {}

	listiter operator--(int) {
		ptr = ptr->p_lastnode;
		return *this;
	}

	listiter operator++(int) {
		ptr = ptr->p_nextnode;
		return *this;
	}
	bool operator!=(const listiter &it) { return ptr != it.ptr; }
	bool operator==(const listiter &it) { return ptr == it.ptr; }
	int operator*() { return ptr->number; }
};

class list
{
	int size;
	listiter beginit;
	listiter endit;
	void freeNodeRecursive(node *p_node);
public:
	list();
	list(int size);
	list(const list &c);
	~list();

	listiter frist();
	listiter last();
	int Size();
	void push(const int n);

	list &operator=(const list &l);
	int operator()();
	list operator+(const int n);
	int &operator[](int idx);
};
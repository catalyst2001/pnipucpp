#pragma once
#include <stdlib.h>
#include <iostream>
//#include <assert.h>

#define assert(x) if(!(x)) __asm int 3h;

void *operator new (size_t size) { return calloc(1, size); };
void operator delete(void *ptr) { free(ptr); }

template<typename _T>
class list
{
	struct node {
		_T data;
		node *p_lastnode;
		node *p_nextnode;
	};

	int size;
	node *firstnode;
	node *lastnode;
	void freeNodeRecursive(node *p_node) {
		if (!p_node)
			return;

		freeNodeRecursive(p_node->p_nextnode); //delete next child node
		delete p_node;
	}

	void initFrom(const list<_T> &c) {
		size = c.size;
		node *p_currnode, *p_node = firstnode;

		// check for empty list
		if (!p_node)
			return;

		// copy first node
		node *p_lastnode = nullptr;
		firstnode = p_currnode = new node;
		p_currnode->data = p_node->data;

		// copy next nodes
		for (p_node = p_node->p_nextnode; p_node; p_node = p_node->p_nextnode) {
			assert(p_node);
			p_currnode->data = p_node->data;
			p_currnode->p_lastnode = p_lastnode;
			p_currnode->p_nextnode = new node; //alloc new node
			p_lastnode = p_currnode; //save last node
			p_currnode = p_currnode->p_nextnode; //switch to new node
		}
		lastnode = p_lastnode;
	}

public:
	list() {
		size = 0;
		firstnode = nullptr;
		lastnode = nullptr;
	}

	list(int size) {
		firstnode = new list::node;
		node *p_current_node = firstnode, *p_last_node = firstnode;
		for (int i = 0; i < size; i++) {
			p_current_node = new node;
			p_current_node->p_lastnode = p_last_node;
			p_last_node->p_nextnode = p_current_node;
		}
		lastnode = p_current_node; //save end node ptr to iter
	}

	list(const list &c) { initFrom(c); }

	~list() {
		freeNodeRecursive(firstnode);
		firstnode = nullptr;
		lastnode = nullptr;
	}

	_T frist() { return firstnode->data; }
	_T last() { return lastnode->data; }

	int Size() { return size; }

	void push(_T p)
	{
		node *p_newnode = new node; // alloc new node to 'next' ptr
		if (!firstnode)
			firstnode = p_newnode;

		if (lastnode)
			lastnode->p_nextnode = p_newnode;

		p_newnode->p_lastnode = lastnode;
		p_newnode->data = p;
		lastnode = p_newnode;
		size++; //increment list size
	}

	_T operator[](int idx)
	{
		if (!firstnode)
			firstnode = new node;

		assert(idx >= 0 && idx < size);
		if (!idx)
			return firstnode->data; //return first node

		int i;
		node *p_node = firstnode;
		for (i = 0; i < idx; i++) {
			assert(p_node->p_nextnode);
			p_node = p_node->p_nextnode;
		}
		return p_node->data;
	}

	int operator()() { return size; }

	list<_T> operator+(_T n) {
		list<_T> temp;
		for (node *p_node = firstnode; p_node; p_node = p_node->p_nextnode)
			temp.push(p_node->data + n);

		return temp;
	}

	list<_T> operator>>(_T n) {
		list<_T> temp;
		for (node *p_node = firstnode; p_node; p_node = p_node->p_nextnode)
			temp.push(p_node->data + n);

		return temp;
	}

	list<_T> &operator=(const list<_T> &from)
	{
		freeNodeRecursive(firstnode);
		firstnode = nullptr;
		lastnode = nullptr;
		initFrom(from);
	}
};
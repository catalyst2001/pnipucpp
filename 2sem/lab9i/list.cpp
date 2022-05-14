#include "list.h"

void *operator new (size_t size) { return calloc(1, size); };
void operator delete(void *ptr) { free(ptr); }

const int maxSize = 10;

void list::freeNodeRecursive(node *p_node)
{
	if (!p_node)
		return;

	freeNodeRecursive(p_node->p_nextnode); //delete next child node
	delete p_node;
}

list::list()
{
	size = 0;
	beginit.ptr = nullptr;
	endit.ptr = nullptr;
}

list::list(int size)
{
	if (!size)
		throw SizeError();

	if (size >= maxSize)
		throw MaxSizeError();

	beginit.ptr = new node;
	node *p_current_node = beginit.ptr, *p_last_node = beginit.ptr;
	for (int i = 0; i < size; i++) {
		p_current_node = new node;
		p_current_node->p_lastnode = p_last_node;
		p_last_node->p_nextnode = p_current_node;
	}
	endit.ptr = p_current_node; //save end node ptr to iter
}

list::list(const list &c)
{
	size = c.size;
	node *p_currnode, *p_node = c.beginit.ptr;

	// check for empty list
	if (!p_node)
		return;

	// copy first node
	node *p_lastnode = nullptr;
	beginit.ptr = p_currnode = new node;
	p_currnode->number = p_node->number;
	
	// copy next nodes
	for (p_node = p_node->p_nextnode; p_node; p_node = p_node->p_nextnode) {
		assert(p_node);
		p_currnode->number = p_node->number;
		p_currnode->p_lastnode = p_lastnode;
		p_currnode->p_nextnode = new node; //alloc new node
		p_lastnode = p_currnode; //save last node
		p_currnode = p_currnode->p_nextnode; //switch to new node
	}
	endit.ptr = p_lastnode;
}

list::~list()
{
	freeNodeRecursive(beginit.ptr);
}

listiter list::frist()
{
	return beginit;
}

listiter list::last()
{
	return endit;
}

int list::Size()
{
	return size;
}

void list::push(const int n)
{
	if (size >= maxSize)
		throw MaxSizeError();

	node *p_newnode = new node; // alloc new node to 'next' ptr
	if (!beginit.ptr)
		beginit.ptr = p_newnode;

	if(endit.ptr)
		endit.ptr->p_nextnode = p_newnode;

	p_newnode->p_lastnode = endit.ptr;
	p_newnode->number = n;
	endit.ptr = p_newnode;
	size++; //increment list size
}

list &list::operator=(const list &l)
{
	if (this == &l)
		return *this;

	if (beginit.ptr) {
		freeNodeRecursive(beginit.ptr);
		size = 0;
	}

	for (node *p_node = l.beginit.ptr; p_node; p_node = p_node->p_nextnode)
		push(p_node->number);

	return *this;
}

int list::operator()()
{
	return size;
}

list list::operator+(const int n)
{
	list temp;
	for (node *p_node = beginit.ptr; p_node; p_node = p_node->p_nextnode)
		temp.push(p_node->number + n);

	return temp;
}

void list::operator++()
{
	push(0);
}

int &list::operator[](int idx)
{
	if (!beginit.ptr)
		beginit.ptr = new node;

	if (!size)
		throw EmptySizeError();

	if (idx < 0)
		throw IndexError1();

	if (idx >= size)
		throw IndexError2();

	if (!idx)
		return beginit.ptr->number; //return first node

	int i;
	node *p_node = beginit.ptr;
	for (i = 0; i < idx; i++) {
		assert(p_node->p_nextnode);
		p_node = p_node->p_nextnode;
	}
	return p_node->number;
}

#include "vector10.h"

const int max_size = 30;

vector10::vector10(int size)
{
	if (size < 1)
		throw SizeError();

	if (size > max_size)
		throw MaxSizeError();

	m_size = size;
	p_data = new int[m_size];
	m_current = 0;
}

vector10::vector10(int *arr, int size)
{
	if (size < 1)
		throw SizeError();

	if (size > max_size)
		throw MaxSizeError();

	m_current = 0; //reset position in dest buffer
	if (m_size != size && p_data) {
		delete[] p_data;
		m_size = size;
		p_data = new int[m_size]; //alloc new memory
	}
	memcpy(p_data, arr, m_size);
}

vector10::vector10(const vector10 &from)
{
	// if dest buffer size less 'from' buffer and dest buffer allocated
	// reallocate buffer and copy data
	m_current = 0; //reset position in dest buffer
	if (m_size != from.m_size && p_data) {
		m_size = from.m_size;
		delete[] p_data; //free old memory
		p_data = new int[m_size]; //alloc new memory
	}
	memcpy(p_data, from.p_data, m_size); //copy data to dest buffer
}

vector10::~vector10()
{
	if (p_data) {
		delete[] p_data;
		p_data = nullptr;
		m_size = 0;
		m_current = 0;
	}
}

int &vector10::operator[](int idx)
{
	if (idx < 0)
		throw IndexError1();
		
	if (idx >= m_size)
		throw IndexError2();

	return p_data[idx];
}

int vector10::operator()()
{
	return m_size;
}

void vector10::operator--()
{
	if (!m_size)
		throw EmptySizeError();

	//shift all elements to left
	for (int i = 0; i < m_size - 1; i++)
		p_data[i] = p_data[i + 1];

	m_size--;
}

void vector10::operator--(int)
{
	if (!m_size)
		throw EmptySizeError();

	m_size--;
}

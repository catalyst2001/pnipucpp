#pragma once
#include <iostream>
#include "IObject.h"
#include <assert.h>

class my_vector
{
	IObject **pp_objects;
	int m_current, m_size;
public:
	my_vector() : pp_objects(nullptr), m_current(0), m_size(0) {};
	my_vector(int size);
	~my_vector();

	bool add(IObject *p_object);
	void clear();

	friend std::ostream& operator<<(std::ostream &out, const my_vector &vec);
};
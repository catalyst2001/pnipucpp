#include "my_vector.h"

my_vector::my_vector(int size)
{
	m_current = 0;
	m_size = size;
	pp_objects = new IObject*[m_size];
	assert(pp_objects);
}

my_vector::~my_vector()
{
	if (pp_objects)
		delete[] pp_objects;
}

bool my_vector::add(IObject *p_object)
{
	if (m_current < m_size) {
		pp_objects[m_current] = p_object;
		m_current++;
		return true;
	}
	return false;
}

void my_vector::clear()
{
	m_current = 0;
}

std::ostream & operator<<(std::ostream &out, const my_vector &vec)
{
	if (!vec.m_size || !vec.m_current) {
		out << "my_vector:: Empty\n";
		return out;
	}

	for (int i = 0; i < vec.m_current; i++)
		vec.pp_objects[i]->show();
	
	return out;
}

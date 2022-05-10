//lab10.cpp
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef unsigned int uint;
typedef unsigned char uchar;

template <typename _type>
class DynamicHeap
{
public:
	DynamicHeap() : m_pData(nullptr), m_nSize(0), m_nOffset(0) {}
	DynamicHeap(int size) : m_pData(nullptr), m_nSize((size > 0) ? size : 1), m_nOffset(0) {
		Realloc(m_nSize);
		assert(m_pData);
	}

	~DynamicHeap() {
		if (m_pData) free(m_pData);
	}

	bool DeleteElement(int index) {
		for (int i = index; i < m_nSize - 1; i++)
			m_pData[i] = m_pData[i + 1];

		m_nSize--;
		if (m_nOffset >= m_nSize)
			m_nOffset--;

		return true;
	}

	void Push(_type elem) {
		CheckAndAutorealloc();
		m_pData[m_nOffset] = elem;
		m_nOffset++;
	}

	int Size() { return m_nSize - 1; }
	int Offset() { return m_nOffset; }

	_type operator[](int ind) {
		assert(ind < m_nSize);
		return m_pData[ind];
	}

	void AddAt(int index, _type *elems, int count) {
		int needed_size = m_nSize + count;
		if (needed_size >= m_nSize) {
			m_nSize = needed_size;
			Realloc(needed_size);
		}
		
		for (int i = Size() - count; i >= index; i--)
			m_pData[i + count] = m_pData[i];
		
		memcpy(&m_pData[index], elems, sizeof(_type) * count);
	}

private:
	inline bool Realloc(int newsize) {
		m_pData = (_type *)realloc(m_pData, newsize * sizeof(_type));
		assert(m_pData);
		return !!m_pData;
	}

	inline void CheckAndAutorealloc() {
		if ((m_nOffset + 1) == m_nSize) {
			m_nSize++;
			Realloc(m_nSize);
		}
	}

	_type *m_pData;
	uint m_nSize;
	uint m_nOffset;
};

int main()
{
	DynamicHeap<int> heap(10);
	for (int i = 0; i < 10; i++)
		heap.Push(i);

	heap.DeleteElement(0);

	int newelems[] = { 1, 1, 1, 1, 1 };
	heap.AddAt(2, newelems, 5);

	for (int i = 0; i < heap.Size(); i++)
		printf("%d ", heap[i]);

	return 0;
}

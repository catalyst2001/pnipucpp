#include "Vector11.h"

//����������
Vector11::~Vector11(void)
{
	if (beg != 0)
		delete[] beg;

	beg = 0;
}

//����������� � �����������
Vector11::Vector11(int n)
{
	beg = new Object*[n];
	cur = 0;
	size = n;
}

//���������� �������, �� ������� ��������� ��������� p � ������
void Vector11::Add()
{
	Object*p;
	//����� �� �������� ���� ��������� �������
	cout << "1.Print" << endl;
	cout << "2.Book" << endl;
	int y;
	cin >> y;
	if (y == 1)//���������� ������� ������ Print
	{
		Print *a = new Print;
		a->Input();//���� �������� ���������
		p = a;
		if (cur < size)
		{
			beg[cur] = p;//���������� � ������
			cur++;
		}
	}
	else if (y == 2) //���������� ������� ������ Book
	{
		Book *b = new Book;
		b->Input();
		p = b;
		if (cur < size)
		{
			beg[cur] = p;
			cur++;
		}
	}
	return;
}

void Vector11::Add(Object *p)
{
	if (cur < size)
	{
		beg[cur] = p;//���������� � ������
		cur++;
	}
}

//�������� �������
void Vector11::Show()
{
	if (cur == 0)
		cout << "Empty" << endl;

	Object **p = beg;//��������� �� ��������� ���� Object
	for (int i = 0; i < cur; i++)
	{
		(*p)->Show();//����� ������ Show() (������� ����������)
		p++;//����������� ��������� �� ��������� ������
	}
}

void Vector11::ShowOne(int n)
{
	if (cur == 0) {
		cout << "Empty" << endl;
		return;
	}

	if(n < 0 || n > cur) {
		cout << "Index out of bounds\n";
		return;
	}
	beg[n]->Show();
}

//��������, ������� ���������� ������ �������
int Vector11::operator ()()
{
	return cur;
}

//�������� �������� �� �������, ������ �� �������������!
void Vector11::Del()
{
	if (cur == 0)
		return;//������

	cur--;
}

void Vector11::HandleEvent(const TEvent&e)
{
	if (e.what == evMessage)
	{
		Object**p = beg;
		for (int i = 0; i < cur; i++)
		{
			(*p)->HandleEvent(e);//����� ������ (������� ����������)
			p++;//����������� ��������� �� ��������� ������
		}
	}
}
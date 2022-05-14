#include "Tree.h"

//����������
Tree::~Tree(void)
{
	if (beg != 0)
		delete[] beg;

	beg = 0;
}

//����������� � �����������
Tree::Tree(int n)
{
	beg = new Object*[n];
	cur = 0;
	size = n;
}

//���������� �������, �� ������� ��������� ��������� p � ������
void Tree::Add()
{
	Object*p;
	//����� �� �������� ���� ��������� �������
	cout << "1.Person" << endl;
	cout << "2.Emloyee" << endl;
	int y;
	cin >> y;
	if (y == 1)//���������� ������� ������ Person
	{
		Person *a = new Person;
		a->Input();//���� �������� ���������
		p = a;
		if (cur < size)
		{
			beg[cur] = p;//���������� � ������
			cur++;
		}
	}
	else if (y == 2) //���������� ������� ������ Emloyee
	{
		Emloyee *b = new Emloyee;
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

void Tree::Add(Object *p)
{
	if (cur < size)
	{
		beg[cur] = p;//���������� � ������
		cur++;
	}
}

//�������� �������
void Tree::Show()
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

void Tree::ShowOne(int n)
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
int Tree::operator ()()
{
	return cur;
}

//�������� �������� �� �������, ������ �� �������������!
void Tree::Del()
{
	if (cur == 0)
		return;//������

	cur--;
}

void Tree::HandleEvent(const TEvent&e)
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
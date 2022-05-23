#include "Dialog.h"

//�����������
Dialog::Dialog(void)
{
	EndState = 0;
	std::cout << "+ - add object to grp" << std::endl;
	std::cout << "- - rem object from grp" << std::endl;
	std::cout << "/ - get" << std::endl;
	std::cout << "q - quit" << std::endl;
	std::cout << "m - mk group" << std::endl;
	std::cout << "z - show one" << std::endl;
}

//����������
Dialog::~Dialog(void)
{
}

//��������� �������
void Dialog::GetEvent(TEvent &event)
{
	string OpInt = "+-?/qamz"; //������ �������� ���� ��������
	string s;
	string param;
	char code;
	cout << '>';
	cin >> s; code = s[0];//������ ������ �������
	if (OpInt.find(code) >= 0)//�������� �� ������ ����� ��������
	{
		event.what = evMessage;
		switch (code)
		{
		case 'm':
			event.command = cmMake;
			break;//������� ������

		case '+':
			event.command = cmAdd;
			break;//�������� ������ � ������

		case '-':
			event.command = cmDel;
			break;//������� ������ �� ������

		case 's':
			event.command = cmShow;
			break;//�������� ������

		//������� ���������� � �������� �������� ������ � ������� k(������ �������
		//	: z k, ��� k � ����� �����)
		case 'z':
			event.command = cmShowone;
			break;

		case 'q':
			event.command = cmQuit;
			break; //����� ������

		case '/':
			event.command = cmGet;
			break;
		}

		//�������� ��������� �������, ���� ��� ����
		if (s.length() > 1)
		{
			param = s.substr(1, s.length() - 1);
			int A = atoi(param.c_str());//����������� ������� � �����
			event.a = A;//���������� � ���������
		}
		else {
			event.a = 10;
		}
		return;
	}
	event.what = evNothing;//������ �������
}

int Dialog::Execute()
{
	TEvent event;
	do {
		EndState = 0;
		GetEvent(event); //�������� �������
		HandleEvent(event); //���������� �������
	} while (!Valid());
	return EndState;
}

int Dialog::Valid()
{
	if (EndState == 0)
		return 0;

	return 1;
}

void Dialog::ClearEvent(TEvent& event)
{
	event.what = evNothing;//������ �������
}

void Dialog::EndExec()
{
	EndState = 1;
}

//���������� �������
void Dialog::HandleEvent(TEvent& event)
{
	if (event.what == evMessage)
	{
		switch (event.command)
		{
		case cmMake://�������� ������
			size = event.a; //������ ������
			if (!size) {
				std::cout << "Ne ukazano chislo elementov" << std::endl;
				ClearEvent(event);//������� �������
				break;
			}
			beg = new Object*[size];//�������� ������ ��� ������ ����������
			cur = 0; //������� �������
			ClearEvent(event);//������� �������
			break;

		case cmAdd://����������
			Add();
			ClearEvent(event);
			break;

		case cmDel:
			Del(); //��������
			ClearEvent(event);
			break;

		case cmShow:
			Show(); //��������
			ClearEvent(event);
			break;

		case cmShowone:
			ShowOne(event.a);
			ClearEvent(event);
			break;

		case cmQuit:
			EndExec(); //�����
			ClearEvent(event);
			break;

		default:
			Vector11::HandleEvent(event);
		};
	};
}
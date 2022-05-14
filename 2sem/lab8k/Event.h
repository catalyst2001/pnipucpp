#pragma once

enum EVENTS
{
	evNothing, //������ �������
	evMessage = 100, //�������� �������

	cmAdd = 1, //�������� ������ � ������
	cmDel, //������� ������ �� ������
	cmGet, //������� ������� ���� ��������
	cmShow, //������� ��� ������
	cmShowone,
	cmMake, //������� ������
	cmQuit = 101 //�����
};

//����� �������
struct TEvent
{
	int what;//��� �������
	union {
		int command;//��� �������
		struct {
			int message;
			int a;//�������� �������
		};
	};
};
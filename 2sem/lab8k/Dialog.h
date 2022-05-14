#pragma once
#include "Event.h"
#include "vector11.h"

class Dialog : public Vector11
{
public:
	Dialog(void);//����������
public:
	virtual      ~Dialog(void);//����������
	virtual void GetEvent(TEvent &event);//�������� �������
	virtual int  Execute();//������� ���� ��������� �������
	virtual void HandleEvent(TEvent& event); //����������
	virtual void ClearEvent(TEvent& event);//�������� �������
	int          Valid();//�������� �������� EndState
	void         EndExec();//��������� ������� ������ �������

protected:
	int EndState;
};


// lab2i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//
#include <iostream>
#include "kvitanciya.h"
#include <time.h>

kvitanciya makeKvitanciya()
{
	int number;
	double summa;
	kvitanciya k;
	std::cout << "enter number: "; std::cin >> number;
	k.setNumber(number);
	std::cout << "enter summa: "; std::cin >> summa;
	k.setSum(summa);

	char date[512];
	time_t rawTime;
	time(&rawTime);
	tm timeInfo;
	localtime_s(&timeInfo, &rawTime);
	sprintf_s(date, sizeof(date), "%d %d %d %d:%d:%d", timeInfo.tm_mday, timeInfo.tm_mon + 1, timeInfo.tm_year + 1900, timeInfo.tm_hour, timeInfo.tm_min, timeInfo.tm_sec);
	k.setDate(std::string(date));
	return k;
}

void printKvitanciya(kvitanciya &k)
{
	k.show();
}

int main()
{
	kvitanciya kvc;
	kvc.show();

	kvitanciya kvc2(1, "08 05 2022 11:21:24", 10000.0);
	kvc2.show();

	kvitanciya kvc3 = kvc2;
	kvc3.setNumber(2);
	kvc3.setDate("03 05 2022 21:00:00");
	kvc3.setSum(14000.0);
	printKvitanciya(kvc3);

	kvc = makeKvitanciya();
	kvc.show();
	return 0;
}
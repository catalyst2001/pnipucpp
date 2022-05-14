// lab10k.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include "Time.h"
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

#include "file_work.h"

int main()
{
	Time p, p1, p2;
	int k, c;
	int nom;
	char file_name[30];
	do
	{ //Меню
		cout << "\n1. Make file";
		cout << "\n2. Print file";
		cout << "\n3. Udalit zapisi s intervalom";
		cout << "\n4. Add record to file";
		cout << "\n5. Uvelichit vse zapisi c zadannym znachenim";
		cout << "\n6. Dobavit K zapizej v nachalo file";
		cout << "\n0. Exit\n";
		cin >> c;
		switch (c)
		{
		case 1:
			cout << "file name?";
			cin >> file_name;
			k = make_file(file_name);
			if (k < 0)
				cout << "Can't make file";

			break;

		case 2:
			cout << "file name?";
			cin >> file_name;
			k = print_file(file_name);
			if (k == 0)
				cout << "Empty file\n";
			if (k < 0)
				cout << "Can't read file\n";
			break;

		case 3:
			cout << "file name?";
			cin >> file_name;

			cout << "Interval 1: "; cin >> p1;
			cout << "Interval 2: "; cin >> p2;
			k = delete_range(file_name, p1, p2);
			if (k < 0) {
				cout << "Can't read file\n";
				break;
			}
			cout << "Udaleno " << k << " strok\n";
			break;

		case 4:
			cout << "file name?";
			cin >> file_name;
			cout << "nom?"; cin >> nom;
			cout << "New time:";
			//Time p1;
			cin >> p1;
			k = add_file(file_name, nom, p1);
			if (k < 0)
				cout << "Can't read file";

			if (k == 0)
				k = add_end(file_name, p1);
			break;

		case 5: /////
			cout << "file name?"; cin >> file_name;
			k = uvelitjit_vse_zapisi_s_znacheniem(file_name);
			if (k < 0)
				cout << "\nCan't read file";

			if (k == 0)
				cout << "\nNot such record";
			break;

		case 6:
			cout << "file name?"; cin >> file_name;
			if (addtofile_begin(file_name) < 0)
				cout << "\nCan't read file";

			break;
		}
	} while (c != 0);
	return 0;
}
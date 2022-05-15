// lab10i.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
#include <iostream>
#include "Pair.h"
#include "file_work.h"

int main()
{
	Pair p, p1, p2;
	int k, c;
	int nom;
	char file_name[30];
	do
	{ //Меню
		cout << "\n1. Make file";
		cout << "\n2. Print file";
		cout << "\n3. Delete all records of greater value"; // !
		cout << "\n4. Add record to file";
		cout << "\n5. Increment all records with a given value by the number L"; // !
		cout << "\n6. Add K records after record number N"; // !
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

			// !
		case 3:
			cout << "file name?";
			cin >> file_name;
			k = delete_greather(file_name, p1);
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
			//Pair p1;
			cin >> p1;
			k = add_file(file_name, nom, p1);
			if (k < 0)
				cout << "Can't read file";

			if (k == 0)
				k = add_end(file_name, p1);
			break;

		// !
		case 5:
			cout << "file name?"; cin >> file_name;
			cout << "Enter direct pair for filter: ";
			cin >> p;
			k = increment_values(file_name, p);
			if (k < 0)
				cout << "\nCan't read file";

			cout << "\n" << k << " records incremented";
			break;

		// !
		case 6:
			cout << "file name?"; cin >> file_name;
			cout << "At line: "; cin >> nom;
			cout << "At line: "; cin >> nom;
			if (addValsAt(file_name, nom, k) < 0)
				cout << "\nCan't read file";

			break;
		}
	} while (c != 0);
}
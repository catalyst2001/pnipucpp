#include "book.h"

Book::Book(void) : Print()
{
	stranic = 0;
	izdatelstvo = "";
}

Book::~Book(void)
{
}

Book::Book(string izd, string name, string auth) : Print(name, auth)
{
	izdatelstvo = izd;
}

Book::Book(const Book &L)
{
	name = L.name;
	author = L.author;
	izdatelstvo = L.izdatelstvo;
	stranic = L.stranic;
}

Book& Book::operator=(const Book&L)
{
	if (&L == this)
		return *this;

	name = L.name;
	author = L.author;
	izdatelstvo = L.izdatelstvo;
	stranic = L.stranic;
	return *this;
}

void Book::Show()
{
	cout << "\nname : " << name;
	cout << "\nauthor : " << author;
	cout << "\nizdatwelstvo : " << izdatelstvo;
	cout << "\nstranicy : " << stranic;
	cout << "\n";
}

void Book::Input()
{
	cout << "\nname : " << name;  cin >> name;
	cout << "\nauthor : " << author; cin >> author;
	cout << "\nizdatwelstvo : " << izdatelstvo; cin >> izdatelstvo;
	cout << "\nstranicy : " << stranic; cin >> stranic;
}
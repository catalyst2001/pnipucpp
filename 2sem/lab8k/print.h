#pragma once
#include "object.h"
#include "Event.h"

class Print : public Object
{
public:
	Print(void);//конструктор без параметров
public:
	virtual ~Print(void);//деструктор

	void Show(); //функция для просмотра атрибутов класса с помощью указателяvoid Input();//функция для ввода значений атрибутов
	void Input();

	Print(std::string name, std::string author);//конструктор с параметрами
	Print(const Print&);//конструктор копирования

	//селекторы
	std::string get_name() { return name; }
	std::string get_author() { return author; }

	//модификаторы
	void set_name(std::string n) { name = n; };
	void set_author(std::string au) { author = au; }
	Print& operator=(const Print&);//перегрузка операции присваивания
	void HandleEvent(const TEvent &e);
protected:
	std::string name;
	std::string author;
};
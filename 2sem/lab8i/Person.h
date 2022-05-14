#pragma once
#include "object.h"
#include "Event.h"

class Person : public Object
{
public:
	Person(void);//конструктор без параметров
public:
	virtual ~Person(void);//деструктор

	void Show(); //функция для просмотра атрибутов класса с помощью указателяvoid Input();//функция для ввода значений атрибутов
	void Input();

	Person(std::string name, int age);//конструктор с параметрами
	Person(const Person&);//конструктор копирования

	//селекторы
	std::string getName() { return name; }
	int getAge() { return age; }
	
	//модификаторы
	void setName(std::string n) { name = n; };
	void setAge(int aa) { age = aa; }
	Person& operator=(const Person&);//перегрузка операции присваивания
	void HandleEvent(const TEvent &e);
protected:
	std::string name;
	int age;
};
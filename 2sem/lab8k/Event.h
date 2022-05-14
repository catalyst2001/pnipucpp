#pragma once

enum EVENTS
{
	evNothing, //пустое событие
	evMessage = 100, //непустое событие

	cmAdd = 1, //добавить объект в группу
	cmDel, //удалить объект из группы
	cmGet, //вывести атрибут всех объектов
	cmShow, //вывести всю группу
	cmShowone,
	cmMake, //создать группу
	cmQuit = 101 //выход
};

//класс событие
struct TEvent
{
	int what;//тип события
	union {
		int command;//код команды
		struct {
			int message;
			int a;//параметр команды
		};
	};
};
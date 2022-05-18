#pragma once
//----------------------------------------------
// GL Developer Library
// Библиотека упрощения разработки демо приложений с визуализацией OpenGL для математических и других задач
// Автор: Дерябин К
// Дата: 05.03.2022
// 
// Построена на старой версии контекстов и не расчитана на использование в реальных проектах!!!
// Плюсы:
// - Проста в использовании
// - Не вынесена в отдельную DLL как glut, glfw, а это значит что код будет встроен в ваше демо приложение без
//   подключения стороннних библиотек. Это улучшает переносимость проекта и не требует дополнительной настройки.
// - Имеет встроенный шрифт для отрисовки текста (сам OpenGL не занимается этим)
// - Имеет встроенные фигуры, такие как сфера и цилиндр (основанные на glu quadrics), ограничивающий прямоугольник, 3D сетка и визуализация осей
//   Сохранение системы координат присутствует и не требует добавления.
//----------------------------------------------
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <gl/GL.h>
#include <gl/GLU.h>
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")

typedef struct gldl_font_s {
	uint32_t display_lists;
} gldl_font_t;

typedef void (*begin2d_fn)();
typedef void (*end2d_fn)();
typedef void (*sphere_fn)(float x, float y, float z, float radius);
typedef void (*cylinder_fn)(float x, float y, float z, float width, float height);
typedef void (*bound_box_fn)(float xmin, float ymin, float zmin, float xmax, float ymax, float zmax);
typedef void (*gird_3d)(float x, float y, float z, float width, float step);
typedef void (*draw_axises)(float line_width, float axis_length);
typedef bool (*is_window_active)();
typedef bool (*is_button_pressed)(int button);

// 
// Отрисовка круга в 2d
// 
#define FILLED 0   //закрашено
#define UNFILLED 1 //не закрашено
typedef void (*draw_2d_circle_fn)(int x, int y, int mode, int nsegs, int radius); //добавлено 17.05.2022

typedef bool (*font_load_fn)(gldl_font_t *p_dstfont, const char *p_fontname, int size, int style);//добавлено 18.05.2022

typedef void (*print_text_fn)(int x, int y, const char *p_text, ...);
typedef long (*int_return_fn)();

typedef struct mouse_point_s {
	long x, y;
} mouse_point_t;
typedef void (*point_ptr_fn)(mouse_point_t *p_dest);

typedef struct gldl_dt_s {

	//начать и завершить отрисовку в ортоганальной проекции
	begin2d_fn        Begin2D;
	end2d_fn          End2D;

	//предустановленные фигуры
	sphere_fn         DrawSphere;
	cylinder_fn       DrawCylinder;
	bound_box_fn      DrawBBox;
	gird_3d           Draw3DGird;
	draw_axises       Draw3DAxises;

	// mode - FILL чтобы закрасить, UNFILL чтобы отрисовать контур
	// nsegs - количество сегментов
	// radius - радиус
	draw_2d_circle_fn Draw2DCircle;

	//функции окна
	is_window_active  IsWindowOpened;
	int_return_fn     GetWindowPosX;
	int_return_fn     GetWindowPosY;
	int_return_fn     GetWindowWidth;
	int_return_fn     GetWindowHeight;
	int_return_fn     GetColorBits;
	int_return_fn     GetDepthBits;

	//нажаты ли кнопки клавиатуры или мыши
	is_button_pressed IsButtonPressed;
	is_button_pressed IsButtonPressedOne;
	int_return_fn     GetMouseWheelDelta;

	//печатает текст на экране

	print_text_fn     PrintText;

	//получает позицию курсора внутри окна
	int_return_fn     GetMouseX;
	int_return_fn     GetMouseY;
	point_ptr_fn      GetMousePosition;
} gldl_dt_t;

//функции событий
typedef struct gldl_events_dt_s {
	void(*p_mouse_move_event)(int x, int y);
	void(*p_mouse_click_event)(int x, int y, int button, int state);
	void(*p_mouse_wheel_event)(int delta);
	void(*p_char_input_event)(int keycode);
	void(*p_keydown_input_event)(int keycode, int state);
	void(*p_window_resize_event)(int width, int height);
} gldl_events_dt_t;

#define LBUTTON 1020 //левая кнопка мыши
#define RBUTTON 1021 //правая кнопка мыши
#define MBUTTON 1022 //центральная кнопка мыши
#define STATE_PRESSED 0 //кнопка нажата
#define STATE_RELEASED 1 //кнопка отпущена

// Коды ошибок
typedef enum gldl_codes_e {
	GLDL_OK = 0, //нет ошибок
	GLDL_ERROR_CREATING_WCLASS,			//не удалось создать класс окна
	GLDL_ERROR_CREATING_WINDOW,			//не удалось создать окно
	GLDL_ERROR_GET_DEVICE_CONTEXT,		//не удалось захватить контекст устройства с окна
	GLDL_ERROR_CHOOSE_PIXELFORMAT,		//не удалось получить совместмый формат пикселей
	GLDL_ERROR_SET_PIXEL_FORMAT,		//не удалось установить формат пикселей
	GLDL_ERROR_CREATING_GL_CONTEXT,		//не удалось создать контекст OpenGL
	GLDL_ERROR_MAKE_CURRENT_CONTEXT,	//не удалось выбрать контекст OpenGL
	GLDL_ERROR_DEINIT					//не удалось правильно освободить ресурсы
} gldl_codes_t;

// 
// gldl_initialize
//  x - позиция x окна на экране. Если x или y выставлены в -1, позиция окна будет подобрана по центру экрана
//  y - позиция y окна на экране. Если x или y выставлены в -1, позиция окна будет подобрана по центру экрана
//  width - ширина окна
//  height - высота окна
//  color_bits - количество бит на пиксель цвета. По умолчанию RGB (24 бита)
//  depth_bits - количество бит буфера глубины. По умолчанию 32 бита
//  p_events_dispatch_table - таблица с функциями получения событий, таких как клик мыши, нажатие кнопок, движение мыши в окне
//                            изменение размера окна. Может не использоваться и быть NULL !
//  
// Инициализирует графическую библиотеку OpenGL и создает окно для отрисовки графики
// При успешном выполнении, функция вернет адрес таблицы функций, доступных для использования
// Если возвращаемое значение равно нулю (NULL), требуется проверить код последней произошедшей ошибки
// 
gldl_dt_t *gldl_initialize(int x, int y, int width, int height, int color_bits, int depth_bits, const char *p_title, gldl_events_dt_t *p_events_dispatch_table);

// 
// gldl_last_error
// 
// Возвращает код последней произошедшей ошибки
// Если ошибок не происходило, возвращаемое значение равно GLDL_OK
// 
gldl_codes_t gldl_last_error();

// 
// gldl_last_error_to_string
// 
// Преобразует код ошибки в строковое описание
// Возвращаемое значение - указатель на строку содержащую описание ошибки
// Если описания к коду ошибки не существует, возвращается "Unknown error"
// 
const char *gldl_last_error_to_string(gldl_codes_t code);

// 
// gldl_last_error_string
// 
// Получить строковое описание последней ошибки
// Возвращаемое значение - указатель на строку содержащую описание ошибки
// 
const char *gldl_last_error_string();

// 
// gldl_shutdown
// 
// Удаление окна и освобождение ресурсов
// Возвращаемое значение при успешном выполнении GLDL_OK
// Возвращаемое значение GLDL_ERROR_DEINIT говорит о произошедшей ошибке при:
//  - освобождении котекста устройства из захвата
//  - удалении контекста визуализации
//  - ошибке удаления окна 
//  - ошибке удаления класса окна
gldl_codes_t gldl_shutdown(gldl_dt_t *p_gldl_window);
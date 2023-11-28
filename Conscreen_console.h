// Compatablility layer for console functions
#pragma  once
#include <stdint.h>
#include <stdbool.h>
#include "Conscreen_ANSI.h"


#if defined(unix)
	#include <termios.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
#elif defined(_WIN32)
	#include <windows.h>
#endif

typedef struct
{
	int16_t x,y;
} Conscreen_point;

typedef struct
{
	Conscreen_point a,b;
} Conscreen_rect;

struct Conscreen_diagnostic
{
	uint8_t check_support: 1;
	uint8_t sgr_support: 1;
	uint8_t color_support: 1;
	uint8_t cursor_move_support: 1;
	uint8_t cursor_visibility_support: 1;
	uint8_t mouse_input_support: 1;
};


#define WRITE(string, size) write(STDOUT_FILENO, string, size)
#define WRITESTR(string) write(STDOUT_FILENO, string, sizeof(string))

struct Conscreen_diagnostic Conscreen_console_check();


// console
Conscreen_point Conscreen_console_size_get();


// Cursor
Conscreen_point Conscreen_console_cursor_get();
void Conscreen_console_cursor_set(Conscreen_point pos);

void Conscreen_console_cursor(bool hide);


// Screenbuffer
void Conscreen_console_clear();
void Conscreen_console_swap_buffer();


// Settings
void Conscreen_console_color(bool enable);
void Conscreen_console_mouse(bool enable);
void Conscreen_console_raw_input(bool enable);
void Conscreen_console_ansi(bool enable);

// Input
char Conscreen_console_get_key();
void Conscreen_console_clear_input();

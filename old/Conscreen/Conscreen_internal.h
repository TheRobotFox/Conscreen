#ifndef _CONSCREEN_INTERNAL_

#define _CONSCREEN_INTERNAL_


#include <stdarg.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include "../DynamicList/DynamicList.h"
#include "Conscreen.h"

// #define DEBUG_MALLOC
// #include "../Debug.h"

#define ESC_COLOR_MAX_LENGTH 19 // E[38;2;255;255;255m
   

#if defined(unix) || defined(__unix__) || defined(__unix)
	#include <termios.h>
	#include <sys/ioctl.h>
	#include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
	#include <windows.h>
#endif


typedef struct Conscreen_screen{
	Conscreen_pixel** pixels;
	unsigned short rows;
	unsigned short columns;
} Conscreen_screen;



// General

void Conscreen_internal_init_check(); // exit if not initilized


int Conscreen_internal_cmp(void* p1, void* p2, int size);
int Conscreen_internal_cmp_pixel(Conscreen_pixel* p1, Conscreen_pixel* p2);

int Conscreen_internal_rgb_to_int(Conscreen_color c);


#define CSI '['
#define cursor_get "6n"
#define coursor_set "H"
#define cursor_v_set "G"
#define screen_clear "2J"
#define screen_cut "J"

void Conscreen_internal_ESC(char type, char* code, ...); // wrapper for ANSI ESC sequences





// Console

#define Conscreen_internal_console_write Conscreen_internal_console_writeA

// manage ANSI console support for windows
#if defined(_WIN32) || defined(_WIN64)
	void Conscreen_internal_console_windows_ANSI_enable(HANDLE h);
	void Conscreen_internal_console_windows_ANSI_disable(HANDLE h);
#endif

// Write string to console
void Conscreen_internal_console_writeA(char* string, int size);
void Conscreen_internal_console_writeW(wchar_t* string, int size);

// Set absolute cursor position
void Conscreen_internal_console_cursor_set(unsigned int row, unsigned int column);
void Conscreen_internal_console_cursor_show(bool show);

// obtain and upadte root_window with console dimensions
u16vec2 Conscreen_internal_console_size_get();

// swap screen buffer; enable ANSI support
int Conscreen_internal_console_setup();
void Conscreen_internal_console_restore();


// Screen

extern Conscreen_screen Screen_buffer;
void Conscreen_internal_screen_clear(Conscreen_screen* S);
void Conscreen_internal_screen_setup();
void Conscreen_internal_screen_cleanup();

void Conscreen_internal_screen_write_pixel(Conscreen_pixel* p);

void Conscreen_internal_screen_update_size();

void Conscreen_internal_screen_interlace(int div, Conscreen_screen* new, Conscreen_screen* old);

void Conscreen_internal_screen_show();

// Printf

int Conscreen_internal_vprintf(const char*const format, va_list arg_list);

int Conscreen_internal_printf(const char*const format, ...);



// ANSI

void Conscreen_internal_ANSI_color_set(Conscreen_color* rgb, unsigned char type);

void Conscreen_internal_ANSI_color_buffer_set(Conscreen_color* rgb, unsigned char type, DList *Output);

void Conscreen_internal_ANSI_attributes_buffer_apply(Conscreen_pixel* p, DList *Output);

void Conscreen_internal_ANSI_attributes_reset();
#endif
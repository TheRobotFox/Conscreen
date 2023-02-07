#ifndef _CONSCREEN

#define _CONSCREEN

#include <stdarg.h>
#include "ConscreenW.h"
#include "../Debug.h"

// #define printf Conscreeen_hook_printf
// #define vprintf Conscreeen_hook_vprintf

// int Conscreeen_hook_printf(const char* format, ...);
// int Conscreeen_hook_vprintf(const char* format, va_list arg_list);



// Functions

int Conscreen_init(); // Initilize
void Conscreen_console_update(); // Resize; Call before drawing frame
u16vec2 Conscreen_size_get(); // Obtain vector with size in character rows and columns of console

void Conscreen_set(unsigned int x, unsigned int y, Conscreen_pixel* p); // set Screenbuffer pixel
void Conscreen_get(unsigned int x, unsigned int y, Conscreen_pixel* p); // read Screenbuffer pixel
void Conscreen_render(); // display Screenbuffer to console

void Conscreen_cleanup(int dummy); // restore console; free buffers

#endif
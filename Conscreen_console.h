// Compatablility layer for console functions

#include <stdint.h>
#include <stdbool.h>

typedef struct
{
	uint16_t x,y;
} Conscreen_point;





void Conscreen_console_check();

void Conscreen_console_screen_set();
void Conscreen_console_screen_flush();


// console dimensions
Conscreen_point Conscreen_console_size_get();


// Cursor
Conscreen_point Conscreen_console_cursor_get();
void Conscreen_console_cursor_set(Conscreen_point pos);

void Conscreen_console_cursor(bool hide);


// switch Screenbuffer
void Conscreen_console_swap_buffer();


// Settings
void Conscreen_console_color(bool enable);
void Conscreen_console_mouse(bool enable);


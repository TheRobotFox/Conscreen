#include <stdint.h>
#include "Conscreen_console.h"
#include "Conscreen_ANSI.h"

typedef struct
{
	Conscreen_char character;
	Conscreen_ansi style;
} Conscreen_pixel;

typedef enum{
	NOTHING,
	REDRAW
} Conscreen_event;

Conscreen_event Conscreen_screen_begin();
void Conscreen_screen_clear();
Conscreen_point Conscreen_screen_size();
void Conscreen_screen_set(uint16_t x, uint16_t y, Conscreen_pixel p);
Conscreen_pixel Conscreen_screen_get(uint16_t x, uint16_t y);
void Conscreen_screen_flush();
void Conscreen_screen_free();

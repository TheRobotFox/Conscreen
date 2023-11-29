#include "Conscreen_screen.h"
#include <assert.h>
#include "Conscreen.h"

static Conscreen_pixel* screen=0;
static Conscreen_point current_size = {0};

Conscreen_event Conscreen_screen_begin()
{
	Conscreen_point new_size = Conscreen_console_size_get();

	if(current_size.x*current_size.y != new_size.x*new_size.y){
		current_size = new_size;
		size_t pixels = new_size.x*new_size.y;
		if(screen)
			Conscreen_screen_free();
		screen = malloc(pixels*sizeof(Conscreen_pixel));

		// Zero out
		Conscreen_pixel p = {.character=0, {.normal=1}};
		for(int i=0; i<pixels; i++)
			screen[i]=p;
		return CS_REDRAW;
	}
	return CS_NOTHING;
}

void Conscreen_screen_clear()
{
	for(int i=0; i<current_size.x*current_size.y; i++)
		screen[i]=(Conscreen_pixel){.style.normal=1, .character = ' '};
	Conscreen_console_clear();
}

Conscreen_point Conscreen_screen_size()
{
	return current_size;
}

void Conscreen_screen_set(uint16_t x, uint16_t y, Conscreen_pixel p)
{
	if(x<current_size.x && y<current_size.y)
		screen[y*current_size.x+x] = p;
}

Conscreen_pixel Conscreen_screen_get(uint16_t x, uint16_t y)
{
	//PRINTF(STR("Conscreen_screen_get %d,%d"), x, y);
	//fflush(stdout);
	if(x<current_size.x && y<current_size.y)
		return screen[y*current_size.x+x];
<<<<<<< HEAD
	//abort();
	return (Conscreen_pixel){0,CONSCREEN_ANSI_NORMAL};
=======
	return (Conscreen_pixel){0};
>>>>>>> 065ca87 (New List)
}

void Conscreen_screen_flush()
{
	Conscreen_string buffer = Conscreen_string_create();

	for(int i=0; i<current_size.x*current_size.y; i++)
	{
		if(i && (i%current_size.x)==0)
			;//Conscreen_string_push(buffer, STR('\n'));

		Conscreen_pixel p = screen[i];
		if(!p.character)
			p.character=CHR(' ');
		Conscreen_ansi_changeB(p.style, buffer);
		Conscreen_string_push(buffer, p.character);
	}
	//Conscreen_console_clear();
	Conscreen_console_cursor_set((Conscreen_point){0,0});
	WRITE(Conscreen_string_start(buffer), Conscreen_string_size(buffer));
}

void Conscreen_screen_free()
{
	free(screen);
}

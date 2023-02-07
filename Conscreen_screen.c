#include "Conscreen_screen.h"

static Conscreen_pixel* screen=0;
static Conscreen_point current_size = {0};

Conscreen_event Conscreen_screen_begin()
{
	Conscreen_point new_size = Conscreen_console_size_get();

	if(current_size.x*current_size.y != new_size.x*new_size.y){
		current_size = new_size;
		if(screen)
			Conscreen_screen_free();
		screen = malloc(new_size.x*new_size.y*sizeof(Conscreen_pixel));
		return REDRAW;
	}
	return NOTHING;
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

void Conscreen_screen_flush()
{
	Conscreen_string buffer = Conscreen_string_create();

	for(int i=0; i<current_size.x*current_size.y; i++)
	{
		if(i && (i%current_size.x)==0)
			Conscreen_string_push(buffer, STR('\n'));

		Conscreen_pixel p = screen[i];
		Conscreen_ansi_changeB(p.style, buffer);
		Conscreen_string_push(buffer, p.character);
	}
	WRITE(Conscreen_string_start(buffer), Conscreen_string_size(buffer));
}

void Conscreen_screen_free()
{
	free(screen);
}

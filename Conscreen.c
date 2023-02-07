#include "Conscreen.h"


bool Conscreen_init()
{
	// setup Console
	Conscreen_console_ansi(true);
	Conscreen_console_raw_input(true);
	Conscreen_console_swap_buffer();

	// Mouse Input
	//
	return false;
}

bool Conscreen_cleanup()
{
	// reset Console
	Conscreen_console_ansi(true);
	Conscreen_console_raw_input(true);
	Conscreen_console_swap_buffer();

	// free memory
	Conscreen_screen_free();

	return false;
}

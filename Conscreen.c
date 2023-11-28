#include "Conscreen.h"


bool Conscreen_init()
{
	// setup Console
	//Conscreen_console_swap_buffer();
	Conscreen_console_ansi(true);
	Conscreen_console_raw_input(true);
	Conscreen_console_cursor(true);

	// Mouse Input
	//
	return false;
}

bool Conscreen_deinit()
{
	// reset Console
	//Conscreen_console_swap_buffer();
	Conscreen_console_ansi(false);
	Conscreen_console_raw_input(false);
	Conscreen_console_cursor(false);
	WRITESTR(STR(ESC CSI "0m\n"));

	// free memory
	Conscreen_screen_free();

	return false;
}

#include <stdint.h>
#ifdef CONSCREEN_WCHAR

	#include <tchar.h>
	typedef wchar_t Conscreen_char;
	//TODO: Set local to UTF-8

#else

	typedef char Conscreen_char;

#endif

typedef struct
{
	uint8_t r,g,b;
} Conscreen_color;

typedef struct
{
	Conscreen_color foreground;
	Conscreen_color background;
	Conscreen_char character;
} Conscreen_pixel;

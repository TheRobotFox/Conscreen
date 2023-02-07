#ifndef _Conscreen_types_

#define _Conscreen_types_
#include <wchar.h>

typedef struct u16vec2{
	unsigned short x,y;
} u16vec2;

typedef struct Conscreen_color{
	unsigned char r,g,b;
} Conscreen_color;


#define Conscreen_RGB(r,g,b) (Conscreen_color){r,g,b}

typedef struct Conscreen_pixel_attributes{
	Conscreen_color foregroundcolor;
	Conscreen_color backgroundcolor;

} Conscreen_pixel_attributes;

#define Conscreen_attri_normal (Conscreen_pixel_attributes){Conscreen_RGB(255,255,255),Conscreen_RGB(0,0,0)}

typedef struct Conscreen_pixel{
	wchar_t c;
	Conscreen_pixel_attributes attributes;
} Conscreen_pixel;

#endif
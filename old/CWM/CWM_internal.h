#ifndef _CWM_INTERNAL_

#define _CWM_INTERNAL_

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "../Conscreen/Conscreen.h"
#include "../DynamicList/DynamicList.h"
#include "CWM.h"

typedef struct f32vec2{
	float x,y;
} f32vec2;

typedef struct CWM_string{
	union{
		wchar_t* wstr;
		char* str;
	};
	unsigned int length;
	Conscreen_pixel_attributes attributes;
} CWM_string;

typedef struct CWM_transformation{
	enum CWM_window_transformation_type type;
	union{
		u16vec2 absolute;
		f32vec2 relative;
	};
} CWM_transformation;

typedef struct CWM_window{
	struct CWM_window* parent;
	DList child_windows;

	struct{
		char show : 1;              	// show window or not
		char border_show: 1;       	// show window border

		CWM_transformation pos; 	// position transformation 
						// ONLY USE CWM_window_pos_get() or CWM_window_pos_get_content() to obtain position

		CWM_transformation size;	// size transformation
						// ONLY USE CWM_window_size_get() or CWM_window_size_get_content() to obtain size

		Conscreen_pixel_attributes border_attribute; // Border color / style

		short Z_depth;
		float opacity;
	} window;

	struct{ 
		u16vec2 cursor;
		wchar_t* input;
	} input;

	struct{
		CWM_string title;                // window title
		CWM_string error;                // window error
		enum CWM_window_content_type type;
		union{
			CWM_string text;
			Conscreen_pixel* pixel_array;
		};

	} content;

} CWM_window;

extern CWM_window* root_window;

void CWM_internal_init_check();


int CWM_internal_min_u16(unsigned short a, unsigned short b);
int CWM_internal_max_u16(short a, short b);
void CWM_internal_string_setW(CWM_string *dst, wchar_t *wstr, int length);

CWM_window* CWM_internal_window_create();

// internal function for creating a template window stuct instance pointer
void CWM_internal_console_write(char* string, int size);

// free window struct 
// Conscreen_internal_window_free_cleanup should always be called after
void CWM_internal_window_free(CWM_window* w);

// remove zero pointers from freed child_windows
void CWM_internal_window_free_cleanup(CWM_window* parent);


void CWM_internal_window_render(CWM_window* w);


#endif
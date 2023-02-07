#ifndef _CWM_

#define _CWM_

#ifndef _Conscreen_types_
#include "../Conscreen/ConscreenW.h"
#endif
#include <stdbool.h>


enum CWM_window_transformation_type{
	CWM_ABSOLUTE,
	CWM_RELATIVE,
	// Conscreen_AUTOMATIC TODO
};

enum CWM_window_content_type{
	CWM_TEXT,
	CWM_COSTUM,
};

typedef struct CWM_window CWM_window;

int CWM_init();

// Obtain rootwindow
CWM_window* CWM_window_root_get();

void CWM_render();

void CWM_error(CWM_window* screen, enum ErrorLevel level, const char*const format, ...);

// get pointer to new child window for give window
CWM_window* CWM_window_push(CWM_window* parent);

// free window and do cleanup
void CWM_window_remove(CWM_window* w);

void CWM_window_title_set(CWM_window* w, wchar_t* title, int length);
void CWM_window_text_set(CWM_window* w, wchar_t* text, int length);
void CWM_window_depth_set(CWM_window* w, int depth);
void CWM_window_pos_set_absolute(CWM_window* w, unsigned short x, unsigned short y);
void CWM_window_pos_set_relative(CWM_window* w, float x, float y);

void CWM_window_size_set_absolute(CWM_window* w, unsigned short x, unsigned short y);
void CWM_window_size_set_relative(CWM_window* w, float x, float y);

void CWM_window_opactity_set(CWM_window* w, float opacity);
void CWM_window_border_show(CWM_window* w, bool show);

u16vec2 CWM_window_size_get(CWM_window* w);
u16vec2 CWM_window_size_get_content(CWM_window* w);
u16vec2 CWM_window_pos_get(CWM_window* w);
u16vec2 CWM_window_pos_get_content(CWM_window* w);

void CWM_cleanup(int dummy);

#endif
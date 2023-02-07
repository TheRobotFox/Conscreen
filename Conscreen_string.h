#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "List/List.h"

//#define CONSCREEN_CHAR
#define CONSCREEN_WCHAR

#ifdef CONSCREEN_WCHAR

	#include <wchar.h>
	typedef wchar_t Conscreen_char;
	//TODO: Set local to UTF-8
	#define _XSTR(x) L##x
	#define STR(x)  _XSTR(x)
	#define CHR(x) _XSTR(x)

	#define STRLEN wcslen
	#define STRNCMP wcsncmp
	#define PUTS(str) wprintf(L"%ls\n", str)
	#define PRINTF wprintf
	#define _STR L"%ls"

#elif defined(CONSCREEN_CHAR)

	typedef char Conscreen_char;
	#define STR(x) x
	#define CHR(x) x

	#define STRLEN strlen
	#define STRNCMP strncmp
	#define PUTS puts
	#define PRINTF printf
	#define _STR "%s"

#else
	#error NO CHARACTER WIDTH SPECIFIED

#endif

typedef List Conscreen_string;


Conscreen_string Conscreen_string_create();
void Conscreen_string_free(Conscreen_string cs_string);
size_t Conscreen_string_size(Conscreen_string cs_string);

Conscreen_char* Conscreen_string_start(Conscreen_string cs_string);
Conscreen_char* Conscreen_string_end(Conscreen_string cs_string);

void Conscreen_string_cat(Conscreen_string cs_string_front, Conscreen_string cs_string_end);
void Conscreen_string_cut(Conscreen_string cs_string, size_t len);
void Conscreen_string_append(Conscreen_string cs_string, const Conscreen_char *chars);
void Conscreen_string_push(Conscreen_string cs_string, Conscreen_char c);
int Conscreen_string_sprintf(Conscreen_string cs_string, const Conscreen_char *fmt, ...);

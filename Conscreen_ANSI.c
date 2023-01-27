#include "Conscreen_ANSI.h"

#define CMP(a,b, field) a.field!=b.field

bool Conscreen_ansi_change(Conscreen_ansi old, Conscreen_ansi new, Conscreen_char buffer, size_t max)
{
	#define COMPARE(field) if(CMP(old, new, field))
	#define EVAL(on, off) 

	COMPARE(normal){
	}

	COMPARE(field);

	#undef COMPARE
}

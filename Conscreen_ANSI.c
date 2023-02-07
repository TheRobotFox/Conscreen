#include "Conscreen_ANSI.h"

void Conscreen_ansi_change(Conscreen_ansi old_ansi, Conscreen_ansi new_ansi, Conscreen_string out)
{
	#define _CMP(a,b, field) ( a.field != b.field )

	#define EVAL(field, on, off)						\
		if(_CMP(new_ansi, old_ansi, field)){						\
			Conscreen_string_append(out, ESC CSI);		\
			if(new_ansi.field)								\
				Conscreen_string_append(out, STR(#on) STR("m"));	\
			else										\
				Conscreen_string_append(out, STR(#off) STR("m"));	\
		}												\

	if(_CMP(old_ansi, new_ansi, normal)){
		if(new_ansi.normal){
			// abort if normal
			Conscreen_string_append(out, ESC CSI STR("0m"));
			return;
		}
	}

	EVAL(invert, 7, 27)
	EVAL(strike, 7, 27)
	EVAL(overline, 53, 5)

	//TODO: multi bit options
	EVAL(underline, 4, 24)
	EVAL(intensity, 2, 22)


	#undef _CMP
	#undef EVAL

	#define _CMP(c1, c2) ((c1.r==c2.r) && (c1.g==c2.g) && (c1.b==c2.b))
	#define _EXP(c) c.r, c.g, c.b

	if(!_CMP(old_ansi.forground, new_ansi.forground))
		Conscreen_string_sprintf(out, ESC CSI STR("38;2;%d;%d;%dm"), _EXP(new_ansi.forground));

	if(!_CMP(old_ansi.background, new_ansi.background))
		Conscreen_string_sprintf(out, ESC CSI STR("48;2;%d;%d;%dm"), _EXP(new_ansi.background));

}

void Conscreen_ansi_changeB(Conscreen_ansi new_ansi, Conscreen_string out)
{
	static Conscreen_ansi current={.nec=1};
	if(current.nec){
		Conscreen_string_append(out, ESC CSI STR("0m"));
		current=Conscreen_ansi_default(225,225,225);
	}
	Conscreen_ansi_change(current, new_ansi, out);
	current=new_ansi;
}

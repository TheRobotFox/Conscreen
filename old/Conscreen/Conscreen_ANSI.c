#include "Conscreen_internal.h"


static Conscreen_pixel_attributes current_attributes={0};
#define Foreground 38
#define Background 48

static void Conscreen_internal_ANSI_color_update_current(Conscreen_color* rgb, unsigned char type){
	switch (type)
	{
	case Foreground:
		current_attributes.foregroundcolor=*rgb;
		break;
	
	case Background:
		current_attributes.backgroundcolor=*rgb;
		break;

	default:
		exit(0);
	}
}

void Conscreen_internal_ANSI_color_buffer_set(Conscreen_color* rgb, unsigned char type, DList *Output){
	int size=swprintf(NULL, 0, L"\033[%d;2;%d;%d;%dm",type, rgb->r, rgb->g, rgb->b); // Get length of Output
	int index=Output->num;
	DList_supply(Output,Output->num+size); // Guarantee enough space

	swprintf(((wchar_t*)Output->data)+index, Output->max-index+1, L"\033[%d;2;%d;%d;%dm",type, rgb->r, rgb->g, rgb->b); // write sequence to Output
	Conscreen_internal_ANSI_color_update_current(rgb,type);
}

void Conscreen_internal_ANSI_color_set(Conscreen_color* rgb, unsigned char type){
	Conscreen_internal_printf("\033[%d;2;%d;%d;%dm",type, rgb->r, rgb->g, rgb->b);
	Conscreen_internal_ANSI_color_update_current(rgb,type);
}

void Conscreen_internal_ANSI_attributes_buffer_apply(Conscreen_pixel* p, DList *Output){
	if(p->c!=' '){
		if(Conscreen_internal_cmp(&p->attributes.foregroundcolor,&current_attributes.foregroundcolor,sizeof(Conscreen_color)))
			Conscreen_internal_ANSI_color_buffer_set(&p->attributes.foregroundcolor, Foreground, Output);
	}

	if(Conscreen_internal_cmp(&p->attributes.backgroundcolor,&current_attributes.backgroundcolor,sizeof(Conscreen_color)))
		Conscreen_internal_ANSI_color_buffer_set(&p->attributes.backgroundcolor, Background, Output);

}

void Conscreen_internal_ANSI_attributes_reset(){
	current_attributes=Conscreen_attri_normal;
	Conscreen_internal_ANSI_color_set(&current_attributes.foregroundcolor,Foreground);
	Conscreen_internal_ANSI_color_set(&current_attributes.backgroundcolor,Background);
}
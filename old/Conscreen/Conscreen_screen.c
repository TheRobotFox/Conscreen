#include "Conscreen_internal.h"


Conscreen_screen Conscreen_internal_screen_create(unsigned short rows, unsigned short columns){
	Conscreen_screen S = {0};

	S.rows=rows;
	S.columns=columns;
	S.pixels=malloc(sizeof(Conscreen_pixel*)*rows);
	for(int i=0; i<rows; i++){
		S.pixels[i]=malloc(sizeof(Conscreen_pixel)*columns);
	}
	Conscreen_internal_screen_clear(&S);
	return S;
}

void Conscreen_internal_screen_clear(Conscreen_screen* S){
	Conscreen_pixel P = {0};
	P.attributes=Conscreen_attri_normal;
	P.c=' ';
	for(int i=0; i<S->rows; i++){
		for(int p=0; p<S->columns; p++)
				S->pixels[i][p]=P;
	}
}

void Conscreen_internal_screen_free(Conscreen_screen* S){
	for(int i=0; i<S->rows; i++){
		free(S->pixels[i]);
	}
	free(S->pixels);
}

void Conscreen_internal_screen_buffer_set(Conscreen_screen* S, unsigned int x, unsigned int y, Conscreen_pixel* p){
	if(y<S->rows && x< S->columns){
		S->pixels[y][x]=*p;
	}else{
		printf("Screen_Buffer set OOB: %d|%d\n", x,y);
	} 
}

void Conscreen_internal_screen_buffer_get(Conscreen_screen* S, unsigned int x, unsigned int y, Conscreen_pixel* p){
	if(y<S->rows && x< S->columns){
		*p=S->pixels[y][x];
	}else{
		printf("Screen_Buffer get OOB: %d|%d\n", x,y);
		Conscreen_pixel null = {0};
		*p=null;
	} 
}

void Conscreen_internal_screen_buffer_convert(Conscreen_screen* S, DList* Output){
	Conscreen_pixel cp;
	Conscreen_internal_ANSI_attributes_reset();
	DList_clear(Output);
	for(int row=0; row<S->rows; row++){
		for(int column=0; column<S->columns; column++){
			Conscreen_internal_screen_buffer_get(S,column,row,&cp);
			Conscreen_internal_ANSI_attributes_buffer_apply(&cp,Output);
			if(cp.c=='\n' || cp.c==0)
				cp.c=' ';
			DList_push(Output,&cp.c);
		}
	}
}



Conscreen_screen Screen_buffer;
DList Output;


void Conscreen_set(unsigned int x, unsigned int y, Conscreen_pixel* p){
	Conscreen_internal_screen_buffer_set(&Screen_buffer,x,y,p);
}
void Conscreen_get(unsigned int x, unsigned int y, Conscreen_pixel* p){
	Conscreen_internal_screen_buffer_get(&Screen_buffer,x,y,p);
}

void Conscreen_internal_screen_setup(){
	Screen_buffer = Conscreen_internal_screen_create(0, 0);
	Output = DList_create(sizeof(wchar_t));
	Conscreen_console_update();
}

void Conscreen_internal_screen_cleanup(){
	Conscreen_internal_screen_free(&Screen_buffer);
	DList_free(&Output);
}

void Conscreen_console_update(){
	u16vec2 size = Conscreen_internal_console_size_get();
	if(size.y!=Screen_buffer.rows || size.x!=Screen_buffer.columns){
		Conscreen_internal_screen_free(&Screen_buffer);
		Screen_buffer = Conscreen_internal_screen_create(size.y, size.x);
		Conscreen_internal_console_cursor_show(false);
	}
}



u16vec2 Conscreen_size_get(){
	u16vec2 size;
	size.x=Screen_buffer.columns;
	size.y=Screen_buffer.rows;
	return size;
}



void Conscreen_render(){
	if(Screen_buffer.rows && Screen_buffer.columns){
		Conscreen_internal_screen_buffer_convert(&Screen_buffer,&Output);
		Conscreen_internal_console_cursor_set(0,0);
		Conscreen_internal_console_writeW(Output.data,Output.num);
		Conscreen_internal_screen_clear(&Screen_buffer);
	}
}
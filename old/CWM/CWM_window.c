#include "CWM_internal.h"

CWM_window* CWM_internal_window_create()
{
	CWM_window* w = malloc(sizeof(CWM_window));

	w->child_windows=DList_create(sizeof(CWM_window*));
	w->parent=0;

	CWM_string tmp_string = {NULL, 0, Conscreen_attri_normal};
	w->content.error=tmp_string;
	w->content.text=tmp_string;
	w->content.title=tmp_string;

	w->window.Z_depth=0;
	w->window.show=1;
	w->window.pos.type=CWM_RELATIVE;
	w->window.size.type=CWM_RELATIVE;
	w->window.border_show=1;
	w->window.opacity=0.5;

	w->window.pos.relative.x=.25;
	w->window.pos.relative.y=.25;
	w->window.size.relative.x=.75;
	w->window.size.relative.y=.75;

	w->window.border_attribute=Conscreen_attri_normal;
	w->window.border_attribute.foregroundcolor=Conscreen_RGB(255,0,255);
	w->window.border_attribute.backgroundcolor=Conscreen_RGB(0,255,255);

	w->content.title.attributes=w->window.border_attribute;

	w->content.text.attributes=Conscreen_attri_normal;
	w->content.text.attributes.foregroundcolor=Conscreen_RGB(255,255,0);

	return w;
}


void CWM_internal_window_free(CWM_window* w)
{
	// recursively free child_windows
	CWM_window** child_windows=w->child_windows.data;
	for(int i=0; i<w->child_windows.num; i++){
		if(child_windows[i])
			CWM_internal_window_free(child_windows[i]);
	}

	// deparentalize
	if(w->parent){
		int index=DList_indexof(&w->parent->child_windows,w);
		if(index!=MISSING){
			CWM_window** siblings = w->parent->child_windows.data;
			siblings[index]=0;
		}
	}

	// free elements
	DList_free(&w->child_windows);
	free(w);

}


void CWM_internal_window_free_cleanup(CWM_window* parent)
{

	CWM_window** child_windows = parent->child_windows.data;
	int i=0;
	while(i<parent->child_windows.num){

		if(child_windows[i]){
			DList_remove(&parent->child_windows,i);
			continue;
		}
		i++;
	}
}


void CWM_window_remove(CWM_window* w)
{
	CWM_internal_init_check();
	CWM_window* parent = w->parent;
	CWM_internal_window_free(w);
	CWM_internal_window_free_cleanup(parent);
}


CWM_window* CWM_window_root_get()
{
	CWM_internal_init_check();
	return root_window;
}


CWM_window* CWM_window_push(CWM_window* parent)
{
	CWM_internal_init_check();
	CWM_window* w = CWM_internal_window_create();
	w->parent=parent;
	DList_push(&parent->child_windows,&w);
	return w;
}

void CWM_window_title_set(CWM_window* w, wchar_t* title, int length)
{
	CWM_internal_string_setW(&w->content.title,title,length);
}

void CWM_window_text_set(CWM_window* w, wchar_t* text, int length)
{
	CWM_internal_string_setW(&w->content.text,text,length);
}

void CWM_window_depth_set(CWM_window* w, int depth)
{
	w->window.Z_depth=depth;
}

void CWM_window_pos_set_absolute(CWM_window* w, unsigned short x, unsigned short y)
{
	w->window.pos.type=CWM_ABSOLUTE;
	w->window.pos.absolute.x=x;
	w->window.pos.absolute.y=y;
}

void CWM_window_pos_set_relative(CWM_window* w, float x, float y)
{
	w->window.pos.type=CWM_RELATIVE;
	w->window.pos.relative.x=x;
	w->window.pos.relative.y=y;
}
void CWM_window_size_set_absolute(CWM_window* w, unsigned short x, unsigned short y)
{
	w->window.size.type=CWM_ABSOLUTE;
	w->window.size.absolute.x=x;
	w->window.size.absolute.y=y;
}

void CWM_window_size_set_relative(CWM_window* w, float x, float y)
{
	w->window.size.type=CWM_RELATIVE;
	w->window.size.relative.x=x;
	w->window.size.relative.y=y;
}

void CWM_window_opactity_set(CWM_window* w, float opacity)
{
	if(opacity<0)
		opacity=0;
	else if(opacity>1)
		opacity=1;
	w->window.opacity=opacity;
}

void CWM_window_border_show(CWM_window* w, bool show){
	w->window.border_show=show;
}

u16vec2 CWM_window_size_get(CWM_window* w)
{
	u16vec2 size;

	if(w){
		u16vec2 parrent_size = CWM_window_size_get_content(w->parent), parrent_pos=CWM_window_pos_get_content(w->parent);
		u16vec2 pos=CWM_window_pos_get(w);
		switch (w->window.size.type)
		{
			case CWM_ABSOLUTE:
			size.x = w->window.size.absolute.x;
			size.y = w->window.size.absolute.y;
			break;
			case CWM_RELATIVE:
			size.x = w->window.size.relative.x*parrent_size.x+.4;
			size.y = w->window.size.relative.y*parrent_size.y+.4;
			break;
		}

		size.x = CWM_internal_min_u16(size.x, parrent_size.x+parrent_pos.x-pos.x);
		size.y = CWM_internal_min_u16(size.y, parrent_size.y+parrent_pos.y-pos.y);
	}else{
		size=Conscreen_size_get();
	}
	return size;
}

u16vec2 CWM_window_size_get_content(CWM_window* w)
{
	u16vec2 size=CWM_window_size_get(w);
	if(w && (size.x || size.y) && w->window.border_show){
		size.x-=2;
		size.y-=2;
	}
	return size;
}

u16vec2 CWM_window_pos_get(CWM_window* w)
{
	u16vec2 pos;

	if(w){
		u16vec2 parrent_pos = CWM_window_pos_get_content(w->parent);

		switch (w->window.pos.type)
		{

			case CWM_ABSOLUTE:
			pos.x = w->window.pos.absolute.x;
			pos.y = w->window.pos.absolute.y;
			break;

			case CWM_RELATIVE:{

			u16vec2 parrent_size = CWM_window_size_get_content(w->parent);
			pos.x = parrent_pos.x + (parrent_size.x * w->window.pos.relative.x);
			pos.y = parrent_pos.y + (parrent_size.y * w->window.pos.relative.y);
			break;
			}

		}
		pos.x = CWM_internal_max_u16(pos.x, parrent_pos.x);
		pos.y = CWM_internal_max_u16(pos.y, parrent_pos.y);
	}else{
		pos.x=0;
		pos.y=0;
	}

	return pos;
}

u16vec2 CWM_window_pos_get_content(CWM_window* w)
{
	u16vec2 pos=CWM_window_pos_get(w);
	if(w){

		if(w->window.border_show){
			pos.x++;
			pos.y++;
		}
	}
	return pos;
}

int CWM_internal_window_text_line_next(wchar_t* text, int length, int width, int index)
{

	int current=0;
	for(int i=0; i<length; i++){

		current++;
		if(text[i-1]=='\n' || current>=width){

			if(i>index)
				return i;
			current=0;
		}
	}
	return -1;
}

int CWM_internal_window_text_lines_count(wchar_t* text, int length, int width, int index)
{

	int count=0;
	do{
		index=CWM_internal_window_text_line_next(text,length,width,index);
		count++;
	}while(index!=-1);
	return count;
}
static Conscreen_color CWM_internal_color_blend(Conscreen_color* a, Conscreen_color* b, float opacity)
{

	Conscreen_color c;
	c.r=a->r*(1-opacity)+b->r*opacity;
	c.g=a->g*(1-opacity)+b->g*opacity;
	c.b=a->b*(1-opacity)+b->b*opacity;
	return c;
}

int CWM_internal_window_text_cropW(wchar_t* wstr, unsigned int length, u16vec2 size)
{
	int start=0;
	while(CWM_internal_window_text_lines_count(wstr, length,size.x,start)>size.y){
		start=CWM_internal_window_text_line_next(wstr,length,size.x,start);
	}
	return start;
}


enum CWM_side{
	CWM_TOP,
	CWM_BOTTOM,
	CWM_LEFT,
	CWM_RIGHT,
};

wchar_t *clamps[4][2] = {
	{L">|", L"|<"},
	{L"<[", L"]>"},
	{L"∨—", L"—∧"},
	{L"∨—", L"—∧"}
};

// TODO: draw string
static void CWM_internal_window_draw_delimiter(CWM_window* w, enum CWM_side side, unsigned int lenth)
{
	wchar_t **clamp;
	u16vec2 pos = CWM_window_pos_get(w), 
		size = CWM_window_size_get(w);
	unsigned short start, *p_pos, max;

	Conscreen_pixel p = {0,Conscreen_attri_normal};

	if(side==CWM_TOP || side==CWM_BOTTOM){
		p_pos = &pos.y;
		max = size.y;
	}else if(side==CWM_LEFT || side==CWM_RIGHT){
		p_pos = &pos.x;
		max = size.y;
	}else{
		printf("Unknown option");
		return;
	}

	clamp = clamps[side];
	start = *p_pos;
	for(int j=0; j<2; j++){
		p_pos -= wcslen(clamp[j]);
		for(int i=0; (p.c=clamp[j][i]); i++){
			Conscreen_set(pos.x, pos.y, &p);
			*p_pos+=1;
		}
	}
}

static void CWM_internal_window_draw_border(CWM_window* w)
{
	u16vec2 pos  = CWM_window_pos_get(w),
		size = CWM_window_size_get(w);

	if(size.x && size.y){

		Conscreen_pixel_attributes attr=w->window.border_attribute;
		Conscreen_pixel corners = {L'+', attr}, vertical = {L'|', attr}, horizontal = {L'—', attr};
		
		Conscreen_set(pos.x,pos.y, &corners);
		Conscreen_set(pos.x+size.x-1, pos.y, &corners);
		Conscreen_set(pos.x, pos.y+size.y-1, &corners);
		Conscreen_set(pos.x+size.x-1, pos.y+size.y-1, &corners);

		for(int i=1; i<size.x-1; i++){
			Conscreen_set(pos.x+i,pos.y,&horizontal);
			Conscreen_set(pos.x+i,pos.y+size.y-1,&horizontal);
		}

		for(int i=1; i<size.y-1; i++){
			Conscreen_set(pos.x,pos.y+i,&vertical);
			Conscreen_set(pos.x+size.x-1,pos.y+i,&vertical);
		}
	}
}

static void CWM_internal_window_draw_title(CWM_window* w)
{

}

static void CWM_internal_window_draw(CWM_window* w)
{
	u16vec2 pos, size;
	// render rim
	if(w->window.border_show){
		// Render title

		int length = w->content.title.length, offset = pos.x + (size.x-length)/2;
		wchar_t* title = w->content.title.wstr;
		Conscreen_pixel tmp;
		tmp.attributes=attr;

		const wchar_t delimiter[] = {L'>',L'|'};
		const int delimiter_length = sizeof(delimiter)/sizeof(delimiter[0]);

		for(int i=0; i<delimiter_length; i++){
			tmp.c=delimiter[i];
			Conscreen_set(offset-pre_l+i,pos.y,&tmp);
		}
		
		for(int i=0; i<suf_l; i++){
			tmp.c=suf[i];
			Conscreen_set(offset+length+i,pos.y,&tmp);
		}
		
		for(int i=0; i<length; i++){

			tmp.c=title[i];
			Conscreen_set(offset+i,pos.y,&tmp);
		}
	}

	size=CWM_window_size_get_content(w); // use window body size
	if(size.x && size.y){

		Conscreen_pixel p;
		wchar_t* text = w->content.text.string;
		pos=CWM_window_pos_get_content(w);

		// Crop text to fit screen
		int start=CWM_internal_window_text_cropW(text,w->content.text.length,size);

		int x=0,    // current x offset
		y=0,        // current y offset
		pause=0;    // pause text output e.g. after '\n' wait to next line
		wchar_t c;

		for(int i=start; y<size.y;){                // for every row
			Conscreen_get(pos.x+x,pos.y+y,&p);
			CWM_internal_color_blend(&p.attributes.backgroundcolor,&w->content.text.attributes.backgroundcolor,w->window.opacity);


			if(i<w->content.text.length && !pause){
				c=text[i++];
				if(c=='\n')
				pause=1;
			}else
				c=' ';


			if(c==' ' && w->window.opacity<1)
				CWM_internal_color_blend(&p.attributes.foregroundcolor,w->window.opacity);
			else{
				p.attributes=w->content.text.attributes;
				p.c=c;
			}


			Conscreen_set(pos.x+x,pos.y+y,&p);
			x++;
			if(x>=size.x){
				x=0;
				y++;
				pause=0;
			}
		}
	}

}

static int CWM_internal_window_depth_cmp(void* a, void* b){
	CWM_window* _a=*(void**)a, *_b=*(void**)b;
	return _a->window.Z_depth>_b->window.Z_depth;
}

void CWM_internal_window_render(CWM_window* w){

	CWM_internal_window_draw(w);
	CWM_window** child_windows = w->child_windows.data;
	DList_QuickSort(&w->child_windows,0,w->child_windows.num-1,CWM_internal_window_depth_cmp);
	for(int i=0; i<w->child_windows.num; i++){
		CWM_internal_window_render(child_windows[i]);
	}
}

void CWM_debug(){
	static int i = 0;
	u16vec2 size = Conscreen_size_get();
	if(i>=size.x)
		i=0;
	Conscreen_pixel_attributes a={0};
	a.foregroundcolor=Conscreen_RGB(255,0,0);
	Conscreen_pixel p= {L'2',a};
	Conscreen_set(i,size.y/2,&p);
	i++;
}

void CWM_render(){
	Conscreen_console_update(); // update Screenbuffer
	// CWM_debug();
	CWM_internal_window_render(root_window); // draw windows

	Conscreen_render(); // display frame
}

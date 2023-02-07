#include "CWM_internal.h"

static struct {
	int initilized;
	void (*cleanup_next)(int);
} status={0};

CWM_window* root_window;

#if defined(unix) || defined(__unix__) || defined(__unix)

static void CWM_internal_signal_cleanup(int dummy){
	CWM_cleanup(dummy);
	if(status.cleanup_next){
		status.cleanup_next(dummy);
	}
	exit(0);
}

#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
static BOOL WINAPI CWM_internal_signal_cleanup(DWORD dummy){
	CWM_cleanup(dummy);
	return 0;
}
#endif

static void CWM_internal_atexit_cleanup(){
	CWM_cleanup(0);
}

void CWM_internal_init_check(){
	if(!status.initilized){
		printf("CWM not initilized!");
		exit(1);
	}
}

int CWM_internal_min_u16(unsigned short a, unsigned short b){
	if(a<b){
		return a;
	}
	return b;
}

int CWM_internal_max_u16(short a, short b){
	if(a>b){
		return a;
	}
	return b;
}


void CWM_internal_string_setW(CWM_string *dst, wchar_t *wstr, int length){
	dst->wstr=realloc(dst->wstr,length*sizeof(wchar_t));
	dst->length=length;
	for(int i=0; i<length; i++)
		dst->wstr[i]=wstr[i];
}

void CWM_cleanup(int dummy){
	CWM_internal_init_check();
	// CWM_window_remove(root_window);
	status.initilized=0;
}

// setup important CWM
int CWM_init(){
	if(!status.initilized){
		status.initilized=1;

		Conscreen_init();
		root_window = CWM_internal_window_create();
		CWM_window_border_show(root_window,0);
		CWM_window_pos_set_absolute(root_window,0,0);
		CWM_window_size_set_relative(root_window,1,1);
		#if defined(unix) || defined(__unix__) || defined(__unix)
		status.cleanup_next=signal(SIGINT,CWM_internal_signal_cleanup);
		#elif defined(_WIN32) || defined(_WIN64)
		SetConsoleCtrlHandler(CWM_internal_signal_cleanup,TRUE);
		#endif
		atexit(CWM_internal_atexit_cleanup);
		return 0;
	}
	// Conscreen_error(root_window, Conscreen_WARNING,"ConScreen already initilized!");
	return 1;
}
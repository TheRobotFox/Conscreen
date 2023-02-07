#include "Conscreen_internal.h"

static struct {
	int initilized;
	void (*cleanup_next)(int);
} status={0};

#if defined(unix) || defined(__unix__) || defined(__unix)

static void Conscreen_internal_signal_cleanup(int dummy){
	Conscreen_cleanup(dummy);
	if(status.cleanup_next){
		status.cleanup_next(dummy);
	}
	exit(0);
}

#elif defined(_WIN32) || defined(_WIN64)
#include <windows.h>
static BOOL WINAPI Conscreen_internal_signal_cleanup(DWORD dummy){
	Conscreen_cleanup(dummy);
	return 0;
}
#endif
static void Conscreen_internal_atexit_cleanup(){
	Conscreen_cleanup(0);
}

void Conscreen_internal_init_check(){
	if(!status.initilized){
		Conscreen_internal_console_windows_ANSI_enable(0);
		printf("[\033[38;2;240;120;120mError\033[0m]\033[38;2;240;120;120mConScreen not initilized!");
		Conscreen_internal_console_windows_ANSI_disable(0);
		exit(1);
	}
}

void Conscreen_internal_ESC(char type, char* code, ...){
	Conscreen_internal_printf("\033%c", type);
	va_list argv;
	va_start(argv,code);
	Conscreen_internal_vprintf(code,argv);
	va_end(argv);
}

int Conscreen_internal_cmp(void* p1, void* p2, int size){
	char* a=p1;
	char* b=p2;
	for(int i=0; i<size; i++){
		if(*a++ != *b++){
			return 1;
		}
	}
	return 0;
}

int Conscreen_internal_cmp_pixel(Conscreen_pixel* p1, Conscreen_pixel* p2){
	if(p1->c==' ' && p2->c==' '){
		return Conscreen_internal_cmp(&p1->attributes.backgroundcolor,&p2->attributes.backgroundcolor,sizeof(Conscreen_color));
	}
	return Conscreen_internal_cmp(p1,p2,sizeof(Conscreen_pixel));
}

int Conscreen_internal_rgb_to_int(Conscreen_color c){
	int n=0;
	n+=c.r<<16;
	n+=c.g<<8;
	n+=c.b;
	return n;
}

// int Conscreen_vprintf(CWM_window* screen, const char*const format, va_list arg_list){

//     unsigned size = vsprintf(NULL,format,arg_list);
//     char* buff =malloc(size+1);
//     vsprintf(buff,format,arg_list);
//     unsigned int start=0;
//     for(int i=0; i<size; i++){
//         if(buff[i]=='\n'){
//             buff[i]=0;
//             Conscreen_internal_ESC(CSI,"%d" cursor_v_set,screen.window.pos);
//             printf();
//             start=i+1;
//         }
//     }
//     free(buff);
// }

// int Conscreen_printf(CWM_window* screen, const char*const format, ...){
//     va_list argv;
//     va_start(argv,format);
//     Conscreen_vprintf(screen,format,argv);
//     va_end(argv);
// }

// int Conscreeen_hook_printf(const char* format, ...){
//     va_list arg_list;
//     va_start(arg_list,format);
//     int result;
//     if(initilized){
//         Conscreen_error(root_window,Conscreen_WARNING,"printf should not be used with Conscreen! use Conscreen_printf instead!");
//         result=Conscreen_vprintf(root_window,format,arg_list);
//     }else{
//         result=vprintf(format,arg_list);
//     }
//     va_end(arg_list);
//     return result;
// }

// int Conscreeen_hook_vprintf(const char* format, va_list arg_list){
//     if(initilized){
//         Conscreen_error(root_window,Conscreen_WARNING,"vprintf should not be used with Conscreen! use Conscreen_vprintf instead!");
//         return Conscreen_vprintf(root_window,format,arg_list);
//     }else
//         return vprintf(format,arg_list);
// }

// void Conscreen_error(CWM_window* screen, enum ErrorLevel level, const char*const format, ...){
//     if(!initilized)
//         Conscreen_internal_console_windows_ANSI_enable(0);


//     printf("[");
//     switch(level){
//         case Conscreen_WARNING:
//             Conscreen_internal_console_write("\033[38;2;200;155;55mWarning",25);
//             break;
//         case Conscreen_ERROR:
//             Conscreen_internal_console_write("\033[38;2;240;120;120mError",24);
//             break;
//         case Conscreen_FATAL:
//             Conscreen_internal_console_write("\033[38;2;255;0;0mFATAL ERROR",26);
//             break;
//         default:
//             Conscreen_internal_console_write("\033[38;2;240;120;120mUnknown Error",32);
//             break;
//     }
//     Conscreen_internal_console_write("\033[0m] ",6);
//     va_list argp;
//     va_start(argp,format);
//     Conscreen_internal_vprintf(format,argp);
//     va_end(argp);


//     if(!initilized)
//         Conscreen_internal_console_windows_ANSI_disable(0);
// }


// restore console screen buffer; free created windows
void Conscreen_cleanup(int dummy){
	Conscreen_internal_init_check();
	if(status.initilized){
		Conscreen_internal_screen_cleanup();
		Conscreen_internal_console_restore();

		status.initilized=0;
	}
	printf("STOP");
}

// setup important Conscreen settings
int Conscreen_init(){
	if(!status.initilized){
		status.initilized=1;

		Conscreen_internal_console_setup();
		Conscreen_internal_screen_setup();
		#if defined(unix) || defined(__unix__) || defined(__unix)
		status.cleanup_next=signal(SIGINT,Conscreen_internal_signal_cleanup);
		#elif defined(_WIN32) || defined(_WIN64)
		SetConsoleCtrlHandler(Conscreen_internal_signal_cleanup,TRUE);
		atexit(Conscreen_internal_atexit_cleanup);
		#endif

		return 0;
	}
	// Conscreen_error(root_window, Conscreen_WARNING,"ConScreen already initilized!");
	return 1;
}
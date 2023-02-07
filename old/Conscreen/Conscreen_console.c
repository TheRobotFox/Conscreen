#include "Conscreen_internal.h"

#if defined(_WIN32) || defined(_WIN64)
static HANDLE Console_buffer_old, Console_buffer;

void Conscreen_internal_console_windows_ANSI_disable(HANDLE h){
	printf("\033[0m");
	if(h==0)
		h=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(h, &dwMode);
	dwMode &= ~0x4;
	SetConsoleMode(h,dwMode);
}

void Conscreen_internal_console_windows_ANSI_enable(HANDLE h){
	if(h==0)
		h=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(h, &dwMode);
	dwMode |= 0x4;
	SetConsoleMode(h,dwMode);
}

#endif





void Conscreen_internal_console_writeA(char* string, int size){
	Conscreen_internal_init_check();
	#if defined(unix) || defined(__unix__) || defined(__unix)
		for(int i=0; i<size; i++)
			putchar(string+i);
	#elif defined(_WIN32) || defined(_WIN64)
		WriteConsoleA(Console_buffer, string, size, NULL, NULL);
	#endif
}

void Conscreen_internal_console_writeW(wchar_t* string, int size){
	Conscreen_internal_init_check();
	#if defined(unix) || defined(__unix__) || defined(__unix)
		for(int i=0; i<size; i++)
			putwchar(string+i);
	#elif defined(_WIN32) || defined(_WIN64)
		WriteConsoleW(Console_buffer, string, size, NULL, NULL);
	#endif
}

void Conscreen_internal_console_cursor_set(unsigned int row, unsigned int column){
	Conscreen_internal_init_check();

	#if defined(unix) || defined(__unix__) || defined(__unix)
	Conscreen_internal_ESC(CSI,"%d;%d" coursor_set, row+1, column+1);

	#elif defined(_WIN32) || defined(_WIN64)
	COORD cursor={column,row};
	SetConsoleCursorPosition(Console_buffer,cursor);
	
	#endif
}

void Conscreen_internal_console_cursor_show(bool show){
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(Console_buffer,&cci);
	cci.bVisible=show;
	SetConsoleCursorInfo(Console_buffer, &cci);
}


u16vec2 Conscreen_internal_console_size_get(){
	Conscreen_internal_init_check();
	// Conscreen_internal_ESC(CSI,screen_clear);
	u16vec2 size={0};
	#if defined(unix) || defined(__unix__) || defined(__unix)
		// Obtain window size
		struct winsize w={0};
		ioctl(STDOUT_FILENO, TIOCGWINSZ, &w);

		size.x=w.ws_col;
		size.y=w.ws_row;

	#elif defined(_WIN32) || defined(_WIN64)

		// Get scrennbufferinfo
		CONSOLE_SCREEN_BUFFER_INFO info={0};
		GetConsoleScreenBufferInfo(Console_buffer,&info);
		// CONSOLE_FONT_INFO font = {0};
		// GetCurrentConsoleFont(Console_buffer,0,&font);
		// COORD fsize= GetConsoleFontSize(Console_buffer,font.nFont);
		size.y=info.srWindow.Bottom-info.srWindow.Top+1;
		size.x=info.srWindow.Right-info.srWindow.Left+1;
		
		COORD newsize, cursor={0};
		// COORD minsize={GetSystemMetrics(SM_CXMIN)/fsize.X,GetSystemMetrics(SM_CYMIN)/fsize.Y};
		COORD minsize={5,5};
		newsize.X= size.x>minsize.X ? size.x : minsize.X;
		newsize.Y= size.y>minsize.Y ? size.y : minsize.Y;

		if((newsize.X>0 && newsize.Y>0)){
			SetConsoleCursorPosition(Console_buffer,cursor);

			SetConsoleScreenBufferSize(Console_buffer,newsize);
		}
		size.x=newsize.X;
		size.y=newsize.Y;
	#else
		// Conscreen_error(Conscreen_FATAL, "Sorry your OS is not supported yet! :(");
		exit(0);
	#endif
	return size;
}


int Conscreen_internal_console_setup(){
	#if defined(unix) || defined(__unix__) || defined(__unix)
		
	#elif defined(_WIN32) || defined(_WIN64)
	Console_buffer_old = GetStdHandle(STD_OUTPUT_HANDLE);
	Console_buffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	Conscreen_internal_console_windows_ANSI_enable(Console_buffer);
	SetConsoleActiveScreenBuffer(Console_buffer);
	#endif
	return 0;
}

void Conscreen_internal_console_restore(){
	#if defined(unix) || defined(__unix__) || defined(__unix)
	

	#elif defined(_WIN32) || defined(_WIN64)
	SetConsoleActiveScreenBuffer(Console_buffer_old);
	CloseHandle(Console_buffer);

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE)); // empty stdin before exit
	#endif
}
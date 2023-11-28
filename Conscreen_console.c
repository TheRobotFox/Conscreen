#include "Conscreen_console.h"
#include <assert.h>
#include "List/List.h"


static struct{
	List buffer;
	size_t index;
} input_buffer;

#ifdef _WIN32
static HANDLE screen_buffer=0;
#endif

static bool raw_input = false;

static void input_buffer_init()
{
	if(!input_buffer.buffer){
		input_buffer.buffer = List_create(sizeof(char));
		input_buffer.index=0;
	}
}

char Conscreen_console_get_key()
{
	input_buffer_init();
	// if buffer is empty/read get new data
	if(input_buffer.index>=List_size(input_buffer.buffer)){
		List_clear(input_buffer.buffer);
		size_t bytes = fread(List_start(input_buffer.buffer), sizeof(char), List_capacity(input_buffer.buffer), stdin);
		if(bytes>0)
			List_resize(input_buffer.buffer, bytes);
		input_buffer.index=0;
	}
	return *(char*)List_get(input_buffer.buffer, input_buffer.index++);
}

struct Conscreen_diagnostic Conscreen_console_check()
{
	struct Conscreen_diagnostic result = {0};

	#define PROBE a=b; b=Conscreen_console_cursor_get()
	#define CHECK_ADVANCE(n) (b.x-a.x+b.y-a.y)==n

	Conscreen_console_swap_buffer();

	// Check if cursor probe is funcional
	Conscreen_point a={0,0},b={0,0};
	PROBE;
	if(b.x<0 || b.y<0)
		return result;

	WRITESTR("A"); // move cursor +1
	PROBE;
	if(CHECK_ADVANCE(1))
		result.check_support=1;
	else
		return result;

	// SGR check
	WRITESTR(ESC CSI "0mB");

	PROBE;
	if(CHECK_ADVANCE(1))
		result.sgr_support=1;


	// run color check
	WRITESTR(ESC CSI "38;2;255;0;0m" ESC CSI "48;2;0;255;0mC");
	WRITESTR(ESC CSI "38;2;255;255;0m" ESC CSI "48;2;0;255;255mD");
	WRITESTR(ESC CSI "0m");


	PROBE;
	if(CHECK_ADVANCE(2))
		result.color_support=1;

	Conscreen_console_cursor_set((Conscreen_point){0,1});

	PROBE;
	if(b.x == 0 && b.y == 1)
		result.cursor_move_support=1;

	Conscreen_console_cursor(true);
	PROBE;
	if(CHECK_ADVANCE(0))
		result.cursor_visibility_support=1;

	Conscreen_console_swap_buffer();

	return result;
}


// console dimensions
Conscreen_point Conscreen_console_size_get()
{
	Conscreen_point size;
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
		size.y=info.srWindow.Bottom-info.srWindow.Top+1;
		size.x=info.srWindow.Right-info.srWindow.Left+1;

		COORD newsize, cursor={0};
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
		static_assert("OS not supported");
	#endif
	return size;
}

// Cursor
Conscreen_point Conscreen_console_cursor_get()
{

	input_buffer_init(); // make sure input buffer avlaiable

	// Setup Terminal
	bool raw_input_backup = raw_input;
	Conscreen_console_raw_input(true);

	WRITESTR(ESC CSI "6n");

	char c;

	// get start of response (ESC)
	while( (c = getchar()) != EOF )
	{
		if(c==*ESC){
			if(getchar()=='[')
				goto success;
			else
				break;
		}else{
			List_push(input_buffer.buffer, &c);
		}
	}
	goto error;

success:
	; // Make this a statement

	// read numbers to buffer
	#define SIZE 16
	char buff[SIZE],
		 *tmp;
	uint8_t index=0;


	while( (c = getchar()) != EOF || index >= SIZE )
	{
		if(c==';')
			break;

		if(index==SIZE)
			goto error;

		buff[index++]=c;
	}
	buff[index]=0;
	Conscreen_point p = {.y = strtol(buff, &tmp, 10)-1};

	index=0;
	while( (c = getchar()) != EOF || index >= SIZE )
	{
		if(c=='R')
			break;
		if(index==SIZE)
			goto error;
		buff[index++]=c;
	}

	buff[index]=0;
	p.x = strtol(buff, &tmp, 10)-1;

	Conscreen_console_raw_input(raw_input_backup);
	return p;

error:
	Conscreen_console_raw_input(raw_input_backup);
	return (Conscreen_point){-1,-1};
}

void Conscreen_console_cursor_set(Conscreen_point pos)
{
#ifdef _WIN32
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(Console_buffer,&cci);
	cci.bVisible=show;
	SetConsoleCursorInfo(Console_buffer, &cci);
#else
	printf(ESC CSI);
	if(pos.y>=0)
		printf("%d", pos.y+1);
	printf(";");
	if(pos.x>=0)
		printf("%d", pos.x+1);
	printf("H");
	fflush(stdout);
#endif
}

void Conscreen_console_cursor(bool hide)
{
#ifdef _WIN32
	CONSOLE_CURSOR_INFO cci;
	GetConsoleCursorInfo(Console_buffer,&cci);
	cci.bVisible=show;
	SetConsoleCursorInfo(Console_buffer, &cci);
#else
	char str[] = ESC CSI "?25";
	str[sizeof(str)-1]=hide ? 'l' : 'h';
	WRITESTR(str);
#endif
}


// Screenbuffer
void Conscreen_console_clear()
{
	WRITESTR(ESC CSI "2J");
}

void Conscreen_console_swap_buffer()
{

	#if defined(unix)
	static bool active=false;
	char str[8] = ESC CSI "?1049";

	str[7]= active ? 'l' : 'h' ;

	WRITE(str, 8);

	active=!active;

	#elif defined(_WIN32)
	static HANDLE prev = GetStdHandle(STD_OUTPUT_HANDLE);
	if(screen_buffer==0){

		screen_buffer = CreateConsoleScreenBuffer(GENERIC_WRITE | GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
		Conscreen_internal_console_windows_ANSI_enable(Console_buffer);
		SetConsoleActiveScreenBuffer(screen_buffer);

	}else{

		SetConsoleActiveScreenBuffer(prev);
		CloseHandle(screen_buffer);
		screen_buffer=0;
	}
	#endif
}

int Conscreen_internal_console_setup(){
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



void Conscreen_console_ansi(bool enable)
{
#ifdef _WIN32
	HANDLE h=GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(h, &dwMode);
	if(enable){
		dwMode |= 0x4;
	}else{
		WRITESTR(ESC CSI "0m");
		dwMode &= ~0x4;
	}
	SetConsoleMode(h,dwMode);
#endif
}

void Conscreen_console_raw_input(bool enable)
{


#ifdef unix
	static struct termios old;
#elif defined(_WIN32)
	static WORD old;
	HANDLE h = GetStdHandle(STD_INPUT_HANDLE);
#endif



	if(!raw_input && enable){
#ifdef unix
		tcgetattr(STDIN_FILENO, &old);
		struct termios new;

#elif defined(_WIN32)
		GetConsoleMode(h, &old);
		WORD new;
#endif



		new=old; // copy settings



#ifdef unix
		new.c_lflag &= ~(ICANON | ECHO); // enable immediate input; stop input echo
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &new);
#elif defined(_WIN32)
		new &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
		SetConsoleOutputCP(CP_UTF8);
		SetConsoleCP(CP_UTF8);
		SetConsoleMode(h, new);
#endif



		raw_input=true;

	}else if(!enable && raw_input){

#ifdef unix
		tcsetattr(STDIN_FILENO, TCSAFLUSH, &old);
#elif defined(_WIN32)
		SetConsoleMode(h, old);
#endif



		raw_input=false;
	}
}

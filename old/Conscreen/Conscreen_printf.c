#include "Conscreen_internal.h"

int Conscreen_internal_vprintf(const char*const format, va_list arg_list){
	unsigned int size = vsnprintf (NULL, 0, format,arg_list);
	char* buff =malloc(size+1);
	vsprintf(buff,format,arg_list);
	Conscreen_internal_console_write(buff, size);
	free(buff);
	return size;
}

int Conscreen_internal_printf(const char*const format, ...){
	va_list argv;
	va_start(argv,format);
	int res = Conscreen_internal_vprintf(format,argv);
	va_end(argv);
	return res;
}

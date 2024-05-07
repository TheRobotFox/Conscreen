#include "Conscreen_string.h"

Conscreen_string Conscreen_string_create()
{
	return List_create(sizeof(Conscreen_char));
}

void Conscreen_string_free(Conscreen_string cs_string)
{
	List_free(cs_string);
}

void Conscreen_string_cut(Conscreen_string cs_string, size_t len)
{
	if(len<Conscreen_string_size(cs_string)){
		LIST_get(Conscreen_char)(cs_string, len) = CHR('\0');
		List_resize(cs_string, len);
	}
}

size_t Conscreen_string_length(Conscreen_string cs_string)
{
	return List_size(cs_string);
}

size_t Conscreen_string_size(Conscreen_string cs_string)
{
	return Conscreen_string_length(cs_string)*sizeof(Conscreen_char);
}

Conscreen_char* Conscreen_string_start(Conscreen_string cs_string)
{
	return List_start(cs_string);
}

Conscreen_char* Conscreen_string_end(Conscreen_string cs_string)
{
	return List_end(cs_string);
}

void Conscreen_string_cat(Conscreen_string cs_string_front, Conscreen_string cs_string_end)
{
	List_push(cs_string_end, NULL); // grow to \0
	List_concat(cs_string_front, cs_string_end);
}

void Conscreen_string_append(Conscreen_string cs_string_front, const Conscreen_char *chars)
{
	// append length
	unsigned long len = STRLEN(chars);

	// reserve required memory
	List_append(cs_string_front, chars, len+1); // grow to size
	List_resize(cs_string_front, -1); // exclude \0
}

void Conscreen_string_push(Conscreen_string cs_string, Conscreen_char c)
{
	List_push(cs_string, &c);
}

int Conscreen_string_vsprintf(Conscreen_string cs_string, const Conscreen_char *fmt, va_list args)
{

	va_list tmp;
	va_copy(tmp, args);

	int res;

	#ifdef CONSCREEN_WCHAR

	size_t space=List_capacity(cs_string)-Conscreen_string_length(cs_string);

	while((res = vswprintf(Conscreen_string_end(cs_string), space, fmt, tmp))<0)
	{
		List_reserve(cs_string, List_capacity(cs_string)*2);
		space=List_capacity(cs_string)-Conscreen_string_length(cs_string);
		va_copy(tmp, args);
	}
	List_append(cs_string, NULL, res); // grow list size

	#else

	int size = vsnprintf(Conscreen_string_end(cs_string), List_capacity(cs_string)-Conscreen_string_size(cs_string), fmt, tmp);
	List_reserve(cs_string, List_size(cs_string)+size+1);
	res = vsnprintf(Conscreen_string_end(cs_string), size+1, fmt, args);
	List_append(cs_string, NULL, size);

	#endif

	return res;
}

int Conscreen_string_sprintf(Conscreen_string cs_string, const Conscreen_char *fmt, ...)
{

	// init va args
	va_list args;
	va_start(args, fmt);

	int res = Conscreen_string_vsprintf(cs_string, fmt, args);
	va_end(args);
	return res;
}

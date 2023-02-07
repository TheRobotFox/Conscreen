CC=gcc

SOURCE:= $(wildcard Conscreen_*.c)\
		 $(wildcard List/*.c)

HEADERS:= $(wildcard Conscreen_*.h)\
		  $(wildcard List/*.h)

OBJECTS:= $(SOURCE:.c=.o)

App: $(OBJECTS) $(HEADERS)
	gcc $(OBJECTS) -o $@

clean:
	rm -rf $(OBJECTS)

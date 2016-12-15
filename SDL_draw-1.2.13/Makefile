CC = gcc
AR = ar

CFLAGS = -g -Wall

OUT = lib/libSDL_draw.a

OBJECTS = obj/SDL_draw.o
SOURCES = $(wildcard src/*.c)

INCLUDE = include

$(shell mkdir -p lib >/dev/null)
$(shell mkdir -p obj >/dev/null)

all: $(OUT)

clean:
	rm -rf lib
	rm -rf obj

$(OUT): $(OBJECTS)
	$(AR) rcs $(OUT) $(OBJECTS)

obj/SDL_draw.o: $(SOURCES) include/SDL_draw.h
	$(CC) -c $(CFLAGS) -I$(INCLUDE) -o $@ src/SDL_draw.c

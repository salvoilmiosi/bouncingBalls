CC = gcc
AR = ar

CFLAGS = -g -Wall

OUT = bin/libSDL_draw.a

OBJECTS = obj/SDL_draw.o
SOURCES = $(wildcard src/*.c)

INCLUDE = include

all: $(OUT)

$(OUT): $(OBJECTS)
	$(AR) rcs $(OUT) $(OBJECTS)

obj/SDL_draw.o: $(SOURCES) include/SDL_draw.h
	$(CC) -c $(CFLAGS) -I$(INCLUDE) -o $@ src/SDL_draw.c

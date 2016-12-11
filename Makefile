CXX = g++
LD = g++
CFLAGS = -g -Wall --std=c++11

LDFLAGS =
LIBS = -lSDL2 -lSDL2_net -lSDL2_image -lSDL2_mixer -lSDL2_ttf

INCLUDE = include
BIN_DIR = bin
OBJ_DIR = obj

OUT_BIN = bouncingBalls

ifeq ($(OS),Windows_NT)
	LDFLAGS += -mwindows -lmingw32 -lSDL2main
	OUT_BIN := $(OUT_BIN).exe
endif

$(shell mkdir -p $(BIN_DIR) >/dev/null)
$(shell mkdir -p $(OBJ_DIR) >/dev/null)

DEPFLAGS = -MT $@ -MMD -MP -MF $(OBJ_DIR)/$*.Td

SDL_draw = SDL_draw-1.2.13/bin/libSDL_draw.a

SOURCES = $(wildcard src/*.cpp)
OBJECTS = $(patsubst src/%,$(OBJ_DIR)/%.o,$(basename $(SOURCES))) $(SDL_draw)

all: $(BIN_DIR)/$(OUT_BIN)

clean:
	rm -rf $(BIN_DIR)
	rm -rf $(OBJ_DIR)

$(BIN_DIR)/$(OUT_BIN): $(OBJECTS)
	$(LD) -o $(BIN_DIR)/$(OUT_BIN) $(OBJECTS) $(LDFLAGS) $(LIBS)

$(SDL_draw):
	make -C SDL_draw-1.2.13

$(OBJ_DIR)/%.o : src/%.cpp
$(OBJ_DIR)/%.o : src/%.cpp $(OBJ_DIR)/%.d
	$(CXX) $(DEPFLAGS) $(CFLAGS) -c -I $(INCLUDE) -o $@ $<
	@mv -f $(OBJ_DIR)/$*.Td $(OBJ_DIR)/$*.d

$(OBJ_DIR)/%.d: ;
.PRECIOUS: $(OBJ_DIR)/%.d

-include $(patsubst src/%,$(OBJ_DIR)/%.d,$(basename $(SOURCES)))
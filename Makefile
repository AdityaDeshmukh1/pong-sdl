# Compiler
CC = gcc

# Compiler flags
CFLAGS = `sdl2-config --cflags` -Iinclude -Wall -Wextra

# Linker flags
LDFLAGS = `sdl2-config --libs` -lSDL2_ttf

# Source files (all .c files in src/ plus main file)
SRC = $(wildcard src/*.c) pong.c

# Object files (replace .c with .o)
OBJ = $(SRC:.c=.o)

# Executable name
OUT = pong

# Default target
all: $(OUT)

# How to build the executable from object files
$(OUT): $(OBJ)
	$(CC) -o $@ $^ $(LDFLAGS)

# How to build object files from source files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to delete object files and executable
clean:
	rm -f $(OBJ) $(OUT)

# Optional: declare phony targets (they don’t produce files)
.PHONY: all clean


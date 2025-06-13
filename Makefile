CC=gcc
CFLAGS=`sdl2-config --cflags`
LDFLAGS=`sdl2-config --libs`
SRC=pong.c
OUT=pong

all:
	$(CC) $(SRC) -o $(OUT) $(CFLAGS) $(LDFLAGS)

clean:
	rm -f $(OUT)


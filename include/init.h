#ifndef INIT_H
#define INIT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern TTF_Font *font;

bool init();
void closeSDL();

#endif // !INIT_H

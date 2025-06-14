#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "game.h"
#include "init.h"

void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *message, int x, int y, SDL_Color textColor);
void renderHUD(int scoreP1, int scoreP2);
void render(Paddle *p1, Paddle *p2, Ball *ball, int scoreP1, int scoreP2); 

#endif // !RENDER_H


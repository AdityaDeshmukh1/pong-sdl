#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>
#include <SDL2/SDL.h>
#include "game.h"

void handleEvents(bool *quit, Paddle *p1, Paddle *p2);

#endif // !INPUT_H

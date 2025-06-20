#ifndef MENU_H
#define MENU_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "init.h"
#include "game.h"

#define NUM_OPTIONS 3

typedef struct {
  const char *label;
  GameState nextState;
} MenuOption;

extern MenuOption menuOptions[NUM_OPTIONS];
extern int selectedOption;

void renderMenu();
void handleMenuEvents(bool *quit, GameState *currentState); 

#endif // !MENU_H

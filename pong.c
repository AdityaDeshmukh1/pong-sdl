#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "include/init.h"
#include "include/game.h"
#include "include/render.h"
#include "include/input.h"
#include "include/menu.h"

int main() {
  if (!init())
    return 1;

  GameState currentState = STATE_MENU;
  Paddle p1, p2;
  Ball ball;
  bool quit  = false;
  int scoreP1 = 0, scoreP2 = 0;

  initGame(&p1, &p2, &ball);

  //  while (!quit) {
  //    handleEvents(&quit, &p1, &p2);
  //    update(&p1, &p2, &ball, &scoreP1, &scoreP2);
  //    render(&p1, &p2, &ball, scoreP1, scoreP2);
  //    SDL_Delay(16);
  //  }

  while (!quit) {
    switch (currentState) {
      case STATE_MENU:
        handleMenuEvents(&quit, &currentState);
        renderMenu();
        break;
      case STATE_PLAYING:
        handleEvents(&quit, &p1, &p2);
        update(&p1, &p2, &ball, &scoreP1, &scoreP2);
        render(&p1, &p2, &ball, scoreP1, scoreP2);
        break;
      case STATE_QUIT:
        quit = 1;
        break;
    }

    SDL_Delay(16);
  }

  closeSDL();
  return 0;
}

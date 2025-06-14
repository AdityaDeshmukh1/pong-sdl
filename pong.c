#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "include/init.h"
#include "include/game.h"
#include "include/render.h"
#include "include/input.h"
#include "include/ai.h"

int main() {
  if (!init())
    return 1;

  Paddle p1, p2;
  Ball ball;
  bool quit  = false;
  int scoreP1 = 0, scoreP2 = 0;

  initGame(&p1, &p2, &ball);

  while (!quit) {
    handleEvents(&quit, &p1, &p2);
    update(&p1, &p2, &ball, &scoreP1, &scoreP2);
    updateAIPaddle(&p2, &ball);
    render(&p1, &p2, &ball, scoreP1, scoreP2);
    SDL_Delay(16);
  }

  closeSDL();
  return 0;
}

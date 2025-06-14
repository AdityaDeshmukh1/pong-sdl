#include "../include/input.h"

void handleEvents(bool *quit, Paddle *p1, Paddle *p2) {
  SDL_Event e;
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      *quit = true;
  }

  const Uint8* state = SDL_GetKeyboardState(NULL);

  // Player 1 controls
  if (state[SDL_SCANCODE_W] && p1->y > 0)
    p1->y -= PADDLE_SPEED; 

  if (state[SDL_SCANCODE_S] && p1->y < SCREEN_HEIGHT - PADDLE_HEIGHT) 
    p1->y += PADDLE_SPEED;

  if (state[SDL_SCANCODE_UP] && p2->y > 0)
    p2->y -= PADDLE_SPEED; 

  if (state[SDL_SCANCODE_DOWN] && p2->y < SCREEN_HEIGHT - PADDLE_HEIGHT) 
    p2->y += PADDLE_SPEED;
}

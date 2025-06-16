#include "../include/utils.h"
#include <SDL2/SDL.h>

void increaseBallSpeed(Ball *ball) {
    (ball->velX)++;
    (ball->velY)++;
}

void handlePaddleCollision(Paddle *p, Ball *ball) {
  SDL_Rect ballRect = {ball->x, ball->y, ball->w, ball->h};
  SDL_Rect pRect = {p->x, p->y, p->w, p->h};

  if (SDL_HasIntersection(&ballRect, &pRect)) {
    ball->velX = -ball->velX;
    increaseBallSpeed(ball);
    }
}

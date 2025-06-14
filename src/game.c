#include "../include/game.h"
#include <SDL2/SDL.h>

void initGame(Paddle *p1, Paddle *p2, Ball *ball) {
  p1->x = 20;
  p1->y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
  p1->w = PADDLE_WIDTH;
  p1->h = PADDLE_HEIGHT;

  p2->x = SCREEN_WIDTH - PADDLE_WIDTH - 20;
  p2->y = (SCREEN_HEIGHT - PADDLE_HEIGHT) / 2;
  p2->w = PADDLE_WIDTH;
  p2->h = PADDLE_HEIGHT;

  ball->x = (SCREEN_WIDTH - BALL_SIZE) / 2;
  ball->y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
  ball->w = BALL_SIZE;
  ball->h = BALL_SIZE;
  ball->velX = BALL_SPEED;
  ball->velY = BALL_SPEED;
}

void update(Paddle *p1, Paddle *p2, Ball *ball, int *scoreP1, int *scoreP2) {

  ball->x += ball->velX;
  ball->y += ball->velY;

  // Bounce off top and bottom walls
  if (ball->y <= 0 || ball->y + BALL_SIZE >= SCREEN_HEIGHT)
    ball->velY = -ball->velY;

  // Bounce off paddles
  SDL_Rect ballRect = {ball->x, ball->y, ball->w, ball->h};
  SDL_Rect p1Rect = {p1->x, p1->y, p1->w, p1->h};
  SDL_Rect p2Rect = {p2->x, p2->y, p2->w, p2->h};

  if (SDL_HasIntersection(&ballRect, &p1Rect) || 
     SDL_HasIntersection(&ballRect, &p2Rect)) {
    ball->velX = -ball->velX;
  }

  if (ball->x <= 0) {
    (*scoreP2)++;
    ball->x = (SCREEN_WIDTH - BALL_SIZE) / 2;
    ball->y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
    ball->velX = -ball->velX;
  }

  else if (ball->x + BALL_SIZE >= SCREEN_WIDTH) {
    (*scoreP1)++;
    ball->x = (SCREEN_WIDTH - BALL_SIZE) / 2;
    ball->y = (SCREEN_HEIGHT - BALL_SIZE) / 2;
    ball->velX = -ball->velX;
  }

}


#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PADDLE_WIDTH 20 
#define PADDLE_HEIGHT 100
#define BALL_SIZE 20
#define PADDLE_SPEED 5
#define BALL_SPEED 5

typedef enum {
  STATE_MENU,
  STATE_PLAYING_1P,
  STATE_PLAYING_2P, 
  STATE_QUIT
} GameState;

typedef struct {
  int x, y, w, h;
} Paddle;

typedef struct {
  int x, y, w, h;
  int velX, velY;
} Ball;

void initGame(Paddle *p1, Paddle *p2, Ball *ball);
void update(Paddle *p1, Paddle *p2, Ball *ball, int *scoreP1, int *scoreP2);

#endif // !GAME_H

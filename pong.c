#include <SDL2/SDL.h>
#include <stdbool.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define PADDLE_WIDTH 20 
#define PADDLE_HEIGHT 100
#define BALL_SIZE 20
#define PADDLE_SPEED 5
#define BALL_SPEED 5

typedef struct {
  int x, y, w, h;
} Paddle;

typedef struct {
  int x, y, w, h;
  int velX, velY;
} Ball;

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;

bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  window = SDL_CreateWindow("Pong in C",
                            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                            SCREEN_WIDTH, SCREEN_HEIGHT,
                            SDL_WINDOW_SHOWN);
  if (!window) {
    printf("Window could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
  if (!renderer) {
    printf("Renderer could not be created! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  return true;
}

void closeSDL() {
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

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

void render(Paddle *p1, Paddle *p2, Ball *ball) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Rect p1Rect = {p1->x, p1->y, p1->w, p1->h};
  SDL_Rect p2Rect = {p2->x, p2->y, p2->w, p2->h};
  SDL_Rect ballRect = {ball->x, ball->y, ball->w, ball->h};

  SDL_RenderFillRect(renderer, &p1Rect);
  SDL_RenderFillRect(renderer, &p2Rect);
  SDL_RenderFillRect(renderer, &ballRect);

  SDL_RenderPresent(renderer);
}

int main() {
  if (!init())
    return 1;

  Paddle p1, p2;
  Ball ball;
  bool quit  = false;

  initGame(&p1, &p2, &ball);

  while (!quit) {
    render(&p1, &p2, &ball);
  }

  return 0;
}

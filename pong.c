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

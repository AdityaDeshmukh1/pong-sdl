#include "../include/init.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdio.h>

SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
TTF_Font *font = NULL;

 bool init() {
  if (SDL_Init(SDL_INIT_VIDEO) > 0) {
    printf("SDL could not initialize! SDL_Error: %s\n", SDL_GetError());
    return false;
  }

  if (TTF_Init() == -1) {
    printf("Failed to initialize TTF: %s\n", TTF_GetError());
    return 1;
  }
  
  font = TTF_OpenFont("fonts/pixel_font.ttf", 28);
  if (font == NULL) {
    printf("Failed to retrieve font: %s\n", SDL_GetError());
    return 1;
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

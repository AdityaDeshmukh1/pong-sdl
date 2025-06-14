#include "../include/render.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


void renderText(SDL_Renderer *renderer, TTF_Font *font, const char *message, int x, int y, SDL_Color textColor) {
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, message, textColor);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

void renderHUD(int scoreP1, int scoreP2) {
  char scoreText[64];
  SDL_Color white = {255, 255, 255, 255};

  sprintf(scoreText, "Player 1: %d", scoreP1);
  renderText(renderer, font, scoreText, 20, 20, white);

  sprintf(scoreText, "Player 2: %d", scoreP2);
  renderText(renderer, font, scoreText, SCREEN_WIDTH - 200, 20, white);
}

void render(Paddle *p1, Paddle *p2, Ball *ball, int scoreP1, int scoreP2) {
  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
  SDL_RenderClear(renderer);

  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Rect p1Rect = {p1->x, p1->y, p1->w, p1->h};
  SDL_Rect p2Rect = {p2->x, p2->y, p2->w, p2->h};
  SDL_Rect ballRect = {ball->x, ball->y, ball->w, ball->h};

  SDL_RenderFillRect(renderer, &p1Rect);
  SDL_RenderFillRect(renderer, &p2Rect);
  SDL_RenderFillRect(renderer, &ballRect);

  renderHUD(scoreP1, scoreP2);
  SDL_RenderPresent(renderer);
}


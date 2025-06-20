#include "../include/menu.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>


MenuOption menuOptions[NUM_OPTIONS] = {
  {"Single-Player", STATE_PLAYING_1P},
  {"Two-Player", STATE_PLAYING_2P},
  {"Quit Game", STATE_QUIT}
};

int selectedOption = 0;

void renderMenuText(SDL_Renderer *renderer, TTF_Font *font, const char *message, int x, int y, SDL_Color textColor) {
  SDL_Surface *textSurface = TTF_RenderText_Solid(font, message, textColor);
  SDL_Texture *textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);

  SDL_Rect textRect = {x, y, textSurface->w, textSurface->h};
  SDL_RenderCopy(renderer, textTexture, NULL, &textRect);

  SDL_FreeSurface(textSurface);
  SDL_DestroyTexture(textTexture);
}

void renderMenu() {
  SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

  SDL_Color white = {255, 255, 255, 255};
  SDL_Color green = {0, 255, 0, 255};
  int x = SCREEN_WIDTH / 2 - 100;

  int y = SCREEN_HEIGHT / 2 - 100;
  for (int i = 0; i < NUM_OPTIONS; i++) {
    SDL_Color color;
    if (selectedOption == i)
      color = green;
    else 
      color = white;

    renderMenuText(renderer, font, menuOptions[i].label, x, y, color);
    y += 50;
  }

  SDL_RenderPresent(renderer);
}

void handleMenuEvents(bool *quit, GameState *currentState) {
  SDL_Event e; 
  while (SDL_PollEvent(&e)) {
    if (e.type == SDL_QUIT)
      *quit = true;

    if (e.type == SDL_KEYDOWN) {
      switch (e.key.keysym.sym) {
        case SDLK_UP:
          selectedOption = (selectedOption - 1 + NUM_OPTIONS) % NUM_OPTIONS;
          break;
        case SDLK_DOWN:
          selectedOption = (selectedOption + 1) % NUM_OPTIONS;
          break;
        case SDLK_RETURN:
          *currentState = menuOptions[selectedOption].nextState;
          break;
      }
    }
  }
}



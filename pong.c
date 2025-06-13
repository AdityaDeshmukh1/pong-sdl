#include <SDL2/SDL_ttf.h>
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
  SDL_Color white = {255, 255, 255};

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
    render(&p1, &p2, &ball, scoreP1, scoreP2);
    SDL_Delay(16);
  }

  closeSDL();
  return 0;
}

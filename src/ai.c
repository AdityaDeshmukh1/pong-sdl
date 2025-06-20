#include "../include/ai.h"

void updateAIPaddle(Paddle *aiPaddle, Ball *ball) {
  aiPaddle->y = ball->y;
}

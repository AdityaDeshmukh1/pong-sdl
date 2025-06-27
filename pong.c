#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "include/init.h"
#include "include/game.h"
#include "include/render.h"
#include "include/input.h"
#include "include/menu.h"
#include "include/ai.h"
#include "include/network.h"

int main() {
  if (!init())
    return 1;

  GameState currentState = STATE_PLAYING_MULTIPLAYER_CLIENT;
  Paddle p1, p2;
  Ball ball;
  bool quit  = false;
  int scoreP1 = 0, scoreP2 = 0;

  NetworkConnection server_conn;
  NetworkConnection client_conn;
  NetworkConnection multiplayer_conn;

  initGame(&p1, &p2, &ball);

  while (!quit) {
    switch (currentState) {
      case STATE_MENU:
        handleMenuEvents(&quit, &currentState);
        renderMenu();
        break;
      case STATE_PLAYING_1P:
        updateAIPaddle(&p2, &ball);
        handleEvents(&quit, &p1, &p2);
        update(&p1, &p2, &ball, &scoreP1, &scoreP2);
        render(&p1, &p2, &ball, scoreP1, scoreP2);
        break;
      case STATE_PLAYING_2P:
        handleEvents(&quit, &p1, &p2);
        update(&p1, &p2, &ball, &scoreP1, &scoreP2);
        render(&p1, &p2, &ball, scoreP1, scoreP2);
        break;
      case STATE_PLAYING_MULTIPLAYER_HOST:
        // Accept client if not already connected
        if (multiplayer_conn.sockfd == 0) {
          multiplayer_conn = accept_client(server_conn);
        }

        // Host handles paddle and ball updates
        handleEvents(&quit, &p1, &p2);
        update(&p1, &p2, &ball, &scoreP1, &scoreP2);

        // Send current game state to client
        struct GameStatePacket {
          Paddle p1;
          Paddle p2;
          Ball ball;
          int scoreP1;
          int scoreP2;
        } packet = { p1, p2, ball, scoreP1, scoreP2 };

        send_data(multiplayer_conn, &packet, sizeof(packet));

        render(&p1, &p2, &ball, scoreP1, scoreP2);
        break;
      case STATE_PLAYING_MULTIPLAYER_CLIENT:
        // Send local paddle position to host
        send_data(client_conn, &p2, sizeof(Paddle));

        // Receive updated game state from host
        struct GameStatePacket recv_packet;
        receive_data(client_conn, &recv_packet, sizeof(recv_packet));

        p1 = recv_packet.p1;
        p2 = recv_packet.p2;
        ball = recv_packet.ball;
        scoreP1 = recv_packet.scoreP1;
        scoreP2 = recv_packet.scoreP2;

        // Handle paddle input for p2 only
        handleEvents(&quit, &p1, &p2);

        render(&p1, &p2, &ball, scoreP1, scoreP2);
        break;
      case STATE_QUIT:
        quit = 1;
        break;
    }
    SDL_Delay(16);
  }
  if (server_conn.sockfd) close_connection(server_conn);
  if (client_conn.sockfd) close_connection(client_conn);
  if (multiplayer_conn.sockfd) close_connection(multiplayer_conn);
  closeSDL();
  return 0;

}

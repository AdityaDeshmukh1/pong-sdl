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

  struct InputPacket {
    int paddleY;
  };

  struct GameStatePacket {
    Paddle p1;
    Paddle p2;
    Ball ball;
    int scoreP1;
    int scoreP2;
  };

  Paddle p1, p2;
  Ball ball;
  bool quit  = false;
  int scoreP1 = 0, scoreP2 = 0;

  NetworkConnection server_conn = {0};
  NetworkConnection client_conn = {0};
  NetworkConnection multiplayer_conn = {0};

  GameState currentState = STATE_MENU;
  int SERVER_FLAG = 0, CLIENT_FLAG = 0;

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
        {
          // Create Server 
          if (!SERVER_FLAG) {
            server_conn = create_server(8080);
            multiplayer_conn.sockfd = 0;  // explicitly clear it
            SERVER_FLAG = 1;
          } else {
            // Accept client if not already connected
            if (multiplayer_conn.sockfd == 0) {
              multiplayer_conn = accept_client(server_conn);
            }

            // Receive input from client (p2.y)
            struct InputPacket input;
            receive_data(multiplayer_conn, &input, sizeof(input));
            p2.y = input.paddleY;

            // Host handles its own paddle and ball updates
            handleEvents(&quit, &p1, NULL); // Moves p1 with local input, p2 already set from client
            update(&p1, &p2, &ball, &scoreP1, &scoreP2);

            // Prepare updated game state packet
            struct GameStatePacket packet = { p1, p2, ball, scoreP1, scoreP2 };

            // Send full updated game state to client
            send_data(multiplayer_conn, &packet, sizeof(packet));

            // Render host view
            render(&p1, &p2, &ball, scoreP1, scoreP2);

          }
        }
        break;

      case STATE_PLAYING_MULTIPLAYER_CLIENT:
        {
          if (!CLIENT_FLAG) {
            client_conn = connect_to_server("192.168.0.113", 8080);
            CLIENT_FLAG = 1;
          } else {
            // Handle local paddle input (for p2 only)
            handleEvents(&quit, NULL, &p2);

            // Send p2.y to server via InputPacket
            struct InputPacket input = { p2.y };
            send_data(client_conn, &input, sizeof(input));

            // Receive updated game state from host
            struct GameStatePacket recv_packet;
            receive_data(client_conn, &recv_packet, sizeof(recv_packet));

            // Update received game state
            p1 = recv_packet.p1;
            ball = recv_packet.ball;
            scoreP1 = recv_packet.scoreP1;
            scoreP2 = recv_packet.scoreP2;

            // Do NOT overwrite p2 — client controls p2 locally

            // Render client view
            render(&p1, &p2, &ball, scoreP1, scoreP2);
          }
        }
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

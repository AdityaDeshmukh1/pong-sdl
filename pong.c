#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <stdbool.h>

#include "include/ai.h"
#include "include/game.h"
#include "include/init.h"
#include "include/input.h"
#include "include/menu.h"
#include "include/network.h"
#include "include/render.h"

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

int main() {
  if (!init())
    return 1;

  Paddle p1, p2;
  Ball ball;
  bool quit = false;
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

    case STATE_PLAYING_MULTIPLAYER_HOST: {
      // Create Server
      if (!SERVER_FLAG) {
        server_conn = create_server(8080);
        SERVER_FLAG = 1;
      }

      struct InputPacket input;
      struct sockaddr_in client_addr;
      socklen_t addr_len = sizeof(client_addr);

      // Non-blocking check for data from client
      int bytes_received =
          recvfrom(server_conn.sockfd, &input, sizeof(input), MSG_DONTWAIT,
                   (struct sockaddr *)&client_addr, &addr_len);

      if (bytes_received > 0) {
        // Update p2 position from client input
        p2.y = input.paddleY;
        multiplayer_conn.sockfd = server_conn.sockfd; // Reuse same socket
        multiplayer_conn.addr = client_addr;          // Store client's address
      }

      // Host handles its own paddle and ball updates
      handleEvents(&quit, &p1, NULL); // Moves p1 with local input
      update(&p1, &p2, &ball, &scoreP1, &scoreP2);

      // Prepare updated game state packet
      struct GameStatePacket packet = {p1, p2, ball, scoreP1, scoreP2};

      // Send game state back to client (if connected)
      if (bytes_received > 0) {
        sendto(server_conn.sockfd, &packet, sizeof(packet), 0,
               (struct sockaddr *)&client_addr, addr_len);
      }

      // Render host view
      render(&p1, &p2, &ball, scoreP1, scoreP2);
    } break;

    case STATE_PLAYING_MULTIPLAYER_CLIENT: {
      if (!CLIENT_FLAG) {
        client_conn = connect_to_server("192.168.0.118", 8080);
        CLIENT_FLAG = 1;
      }

      // Handle local paddle input (for p2 only)
      handleEvents(&quit, NULL, &p2);

      // Send p2.y to server via InputPacket
      struct InputPacket input = {p2.y};
      int sent_bytes = sendto(client_conn.sockfd, &input, sizeof(input), 0,
                              (struct sockaddr *)&client_conn.addr,
                              sizeof(client_conn.addr));
      if (sent_bytes < 0) {
        perror("[ERROR] sendto failed");
      }

      // Receive updated game state from host
      struct GameStatePacket recv_packet;
      struct sockaddr_in server_addr;
      socklen_t addr_len = sizeof(server_addr);

      int bytes_received =
          recvfrom(client_conn.sockfd, &recv_packet, sizeof(recv_packet), 0,
                   (struct sockaddr *)&server_addr, &addr_len);
      if (bytes_received < 0) {
        perror("[ERROR] recvfrom failed");
      } else {
        // Update received game state
        p1 = recv_packet.p1;
        ball = recv_packet.ball;
        scoreP1 = recv_packet.scoreP1;
        scoreP2 = recv_packet.scoreP2;
      }

      // Render client view
      render(&p1, &p2, &ball, scoreP1, scoreP2);
    } break;
    case STATE_QUIT:
      quit = 1;
      break;
    }

    SDL_Delay(16); // Delay after every frame
  }

  // Clean up sockets and SDL subsystems after game loop ends
  if (server_conn.sockfd)
    close_connection(server_conn);
  if (client_conn.sockfd)
    close_connection(client_conn);
  if (multiplayer_conn.sockfd)
    close_connection(multiplayer_conn);

  closeSDL();
  return 0;
}


#include "network.h"

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

// Create and bind a server socket on the specified port
NetworkConnection create_server(int port) {
  NetworkConnection server;
  int opt = 1;

  printf("[INFO] Creating UDP server socket on port %d...\n", port);

  server.sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (server.sockfd < 0) {
    perror("[ERROR] socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server.sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    perror("[ERROR] setsockopt failed");
    exit(EXIT_FAILURE);
  }

  memset(&server.addr, 0, sizeof(server.addr));
  server.addr.sin_family = AF_INET;
  server.addr.sin_addr.s_addr = INADDR_ANY;
  server.addr.sin_port = htons(port);

  if (bind(server.sockfd, (struct sockaddr *)&server.addr, sizeof(server.addr)) < 0) {
    perror("[ERROR] bind failed");
    exit(EXIT_FAILURE);
  }

  printf("[INFO] UDP server socket ready (fd: %d)\n", server.sockfd);
  return server;
}

// Setup a client connection to send UDP packets
NetworkConnection connect_to_server(const char *ip, int port) {
  NetworkConnection client;

  printf("[INFO] Setting up UDP client to %s:%d...\n", ip, port);

  client.sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (client.sockfd < 0) {
    perror("[ERROR] socket failed");
    exit(EXIT_FAILURE);
  }

  memset(&client.addr, 0, sizeof(client.addr));
  client.addr.sin_family = AF_INET;
  client.addr.sin_port = htons(port);

  if (inet_pton(AF_INET, ip, &client.addr.sin_addr) <= 0) {
    perror("[ERROR] Invalid address / Address not supported");
    exit(EXIT_FAILURE);
  }

  printf("[INFO] UDP client socket ready (fd: %d)\n", client.sockfd);
  return client;
}

// Send data via UDP
int send_data(NetworkConnection conn, void *data, size_t size) {
  if (conn.sockfd <= 0) {
    fprintf(stderr, "[ERROR] send_data called with invalid socket fd (%d)\n", conn.sockfd);
    return -1;
  }

  int sent_bytes = sendto(conn.sockfd, data, size, 0,
                          (struct sockaddr *)&conn.addr, sizeof(conn.addr));
  if (sent_bytes < 0) {
    perror("[ERROR] sendto failed");
  } else {
    printf("[INFO] Sent %d bytes via UDP (fd: %d)\n", sent_bytes, conn.sockfd);
  }

  return sent_bytes;
}

// Receive data via UDP
int receive_data(NetworkConnection conn, void *buffer, size_t size, struct sockaddr_in *source_addr) {
  if (conn.sockfd <= 0) {
    fprintf(stderr, "[ERROR] receive_data called with invalid socket fd (%d)\n", conn.sockfd);
    return -1;
  }

  socklen_t addr_len = sizeof(struct sockaddr_in);
  int received_bytes = recvfrom(conn.sockfd, buffer, size, 0,
                                (struct sockaddr *)source_addr, &addr_len);
  if (received_bytes < 0) {
    perror("[ERROR] recvfrom failed");
  } else {
    char src_ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &source_addr->sin_addr, src_ip, INET_ADDRSTRLEN);
    printf("[INFO] Received %d bytes from %s:%d (fd: %d)\n",
           received_bytes, src_ip, ntohs(source_addr->sin_port), conn.sockfd);
  }

  return received_bytes;
}

// Close a connection
void close_connection(NetworkConnection conn) {
  if (conn.sockfd > 0) {
    printf("[INFO] Closing socket (fd: %d)\n", conn.sockfd);
    close(conn.sockfd);
  } else {
    printf("[INFO] Attempted to close invalid fd (%d)\n", conn.sockfd);
  }
}


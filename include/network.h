
#ifndef NETWORK_H
#define NETWORK_H

#include <netinet/in.h>

// Struct to hold socket details
typedef struct {
  int sockfd;
  struct sockaddr_in addr;
} NetworkConnection;

// Server Function Prototypes

/**
 * Creates and binds a UDP server socket on the specified port.
 * Returns a NetworkConnection with the server socket descriptor.
 */
NetworkConnection create_server(int port);

/**
 * Sends raw data to the specified address using UDP.
 * Returns number of bytes sent.
 */
int send_data(NetworkConnection conn, void *data, size_t size);

/**
 * Receives raw data over UDP.
 * If source_addr is not NULL, the address of the sender is written into it.
 * Returns number of bytes received.
 */
int receive_data(NetworkConnection conn, void *buffer, size_t size,
                 struct sockaddr_in *source_addr);

// Client Function Prototypes

/**
 * Prepares a UDP client socket to send to a given server IP and port.
 * Returns a NetworkConnection with the configured socket and address.
 */
NetworkConnection connect_to_server(const char *ip, int port);

// Common

/**
 * Closes a socket.
 */
void close_connection(NetworkConnection conn);

#endif // NETWORK_H

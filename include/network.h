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
 * Creates and binds a server socket on the specified port.
 * Returns a NetworkConnection with the server socket descriptor.
 */
NetworkConnection create_server(int port);

/**
 * Waits for a client connection on the given server socket.
 * Returns a NetworkConnection for the connected client.
 */
NetworkConnection accept_client(NetworkConnection server);

/**
 * Sends raw data to a connected client.
 * Returns number of bytes sent.
 */
int send_data(NetworkConnection conn, void *data, size_t size);

/**
 * Receives raw data from a connected client.
 * Returns number of bytes received.
 */
int receive_data(NetworkConnection conn, void *buffer, size_t size);

// Client Function Prototypes

/**
 * Connects to a server at the given IP and port.
 * Returns a NetworkConnection with the connected socket.
 */
NetworkConnection connect_to_server(const char *ip, int port);

// Common

/**
 * Closes a socket connection.
 */
void close_connection(NetworkConnection conn);

#endif // NETWORK_H


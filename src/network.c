#include "network.h"

#include <stdio.h>
#include <string.h>
#include <unistd.h>     // for close()
#include <stdlib.h>     // for exit()
#include <arpa/inet.h>  // for inet_pton(), htons()

// Create and bind a server socket on the specified port
NetworkConnection create_server(int port) {
    NetworkConnection server;
    int opt = 1;

    printf("[INFO] Creating server socket on port %d...\n", port);

    // Create socket: IPv4, TCP
    server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.sockfd < 0) {
        perror("[ERROR] socket failed");
        exit(EXIT_FAILURE);
    }
    printf("[INFO] Server socket created (fd: %d)\n", server.sockfd);

    // Allow reuse of address/port
    if (setsockopt(server.sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("[ERROR] setsockopt failed");
        exit(EXIT_FAILURE);
    }
    printf("[INFO] Socket options set (SO_REUSEADDR)\n");

    // Setup server address
    memset(&server.addr, 0, sizeof(server.addr));
    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = INADDR_ANY;
    server.addr.sin_port = htons(port);

    // Bind socket
    if (bind(server.sockfd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
        perror("[ERROR] bind failed");
        exit(EXIT_FAILURE);
    }
    printf("[INFO] Socket bound to port %d\n", port);

    // Listen for connections
    if (listen(server.sockfd, 1) < 0) {
        perror("[ERROR] listen failed");
        exit(EXIT_FAILURE);
    }
    printf("[INFO] Server listening for connections...\n");

    return server;
}

// Wait for a client connection
NetworkConnection accept_client(NetworkConnection server) {
    NetworkConnection client;
    socklen_t addr_len = sizeof(client.addr);

    printf("[INFO] Waiting to accept a client connection...\n");

    client.sockfd = accept(server.sockfd, (struct sockaddr*)&client.addr, &addr_len);
    if (client.sockfd < 0) {
        perror("[ERROR] accept failed");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Client connected (fd: %d)\n", client.sockfd);

    return client;
}

// Connect to a server at given IP and port
NetworkConnection connect_to_server(const char *ip, int port) {
    NetworkConnection client;

    printf("[INFO] Connecting to server %s:%d...\n", ip, port);

    // Create socket
    client.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client.sockfd < 0) {
        perror("[ERROR] socket failed");
        exit(EXIT_FAILURE);
    }
    printf("[INFO] Client socket created (fd: %d)\n", client.sockfd);

    // Setup server address
    memset(&client.addr, 0, sizeof(client.addr));
    client.addr.sin_family = AF_INET;
    client.addr.sin_port = htons(port);

    // Convert IP string to binary form
    if (inet_pton(AF_INET, ip, &client.addr.sin_addr) <= 0) {
        perror("[ERROR] Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client.sockfd, (struct sockaddr*)&client.addr, sizeof(client.addr)) < 0) {
        perror("[ERROR] connect failed");
        exit(EXIT_FAILURE);
    }

    printf("[INFO] Successfully connected to server %s:%d (fd: %d)\n", ip, port, client.sockfd);

    return client;
}

// Send data over an existing connection
int send_data(NetworkConnection conn, void *data, size_t size) {
    if (conn.sockfd <= 0) {
        fprintf(stderr, "[ERROR] send_data called with invalid socket fd (%d)\n", conn.sockfd);
        return -1;
    }

    int sent_bytes = send(conn.sockfd, data, size, 0);
    if (sent_bytes < 0) {
        perror("[ERROR] send failed");
    } else {
        printf("[INFO] Sent %d bytes to fd %d\n", sent_bytes, conn.sockfd);
    }
    return sent_bytes;
}

// Receive data over an existing connection
int receive_data(NetworkConnection conn, void *buffer, size_t size) {
    if (conn.sockfd <= 0) {
        fprintf(stderr, "[ERROR] receive_data called with invalid socket fd (%d)\n", conn.sockfd);
        return -1;
    }

    int received_bytes = recv(conn.sockfd, buffer, size, 0);
    if (received_bytes < 0) {
        perror("[ERROR] recv failed");
    } else {
        printf("[INFO] Received %d bytes from fd %d\n", received_bytes, conn.sockfd);
    }
    return received_bytes;
}

// Close a connection
void close_connection(NetworkConnection conn) {
    if (conn.sockfd > 0) {
        printf("[INFO] Closing connection (fd: %d)\n", conn.sockfd);
        close(conn.sockfd);
    } else {
        printf("[INFO] Attempted to close invalid fd (%d)\n", conn.sockfd);
    }
}


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

    // Create socket: IPv4, TCP
    server.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (server.sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Allow reuse of address/port
    if (setsockopt(server.sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    memset(&server.addr, 0, sizeof(server.addr));
    server.addr.sin_family = AF_INET;
    server.addr.sin_addr.s_addr = INADDR_ANY; // Accept from any local IP
    server.addr.sin_port = htons(port);

    // Bind socket
    if (bind(server.sockfd, (struct sockaddr*)&server.addr, sizeof(server.addr)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    // Listen for connections (max 1 pending connection)
    if (listen(server.sockfd, 1) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }

    return server;
}

// Wait for a client connection
NetworkConnection accept_client(NetworkConnection server) {
    NetworkConnection client;
    socklen_t addr_len = sizeof(client.addr);

    client.sockfd = accept(server.sockfd, (struct sockaddr*)&client.addr, &addr_len);
    if (client.sockfd < 0) {
        perror("accept failed");
        exit(EXIT_FAILURE);
    }

    return client;
}

// Connect to a server at given IP and port
NetworkConnection connect_to_server(const char *ip, int port) {
    NetworkConnection client;

    // Create socket
    client.sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (client.sockfd < 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Setup server address
    memset(&client.addr, 0, sizeof(client.addr));
    client.addr.sin_family = AF_INET;
    client.addr.sin_port = htons(port);

    // Convert IP string to binary form
    if (inet_pton(AF_INET, ip, &client.addr.sin_addr) <= 0) {
        perror("Invalid address / Address not supported");
        exit(EXIT_FAILURE);
    }

    // Connect to server
    if (connect(client.sockfd, (struct sockaddr*)&client.addr, sizeof(client.addr)) < 0) {
        perror("connect failed");
        exit(EXIT_FAILURE);
    }

    return client;
}

// Send data over an existing connection
int send_data(NetworkConnection conn, void *data, size_t size) {
    int sent_bytes = send(conn.sockfd, data, size, 0);
    if (sent_bytes < 0) {
        perror("send failed");
    }
    return sent_bytes;
}

// Receive data over an existing connection
int receive_data(NetworkConnection conn, void *buffer, size_t size) {
    int received_bytes = recv(conn.sockfd, buffer, size, 0);
    if (received_bytes < 0) {
        perror("recv failed");
    }
    return received_bytes;
}

// Close a connection
void close_connection(NetworkConnection conn) {
    close(conn.sockfd);
}


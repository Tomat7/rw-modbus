// telnet.cpp

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstring> // For memset

#include "telnet.h"

int TelnetSvc_c::init_socket()
{
  // 1. Create a listening socket
  server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket < 0) {
    std::cerr << "Error creating listening socket" << std::endl;
    return 1;
  }

  // Set socket options to reuse address
  int opt = 1;
  if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    std::cerr << "Error setting socket options" << std::endl;
    close(server_socket);
    return 1;
  }

};

int telnet_fn()
{

  init();


  // Bind the listening socket to an address and port
  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT);

  if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error binding socket" << std::endl;
    close(server_socket);
    return 1;
  }

  // Listen for incoming connections
  if (listen(server_socket, 5) < 0) {
    std::cerr << "Error listening on socket" << std::endl;
    close(server_socket);
    return 1;
  }

  std::cout << "Server listening on port " << PORT << std::endl;

  // Initialize pollfd structures
  std::vector<pollfd> array_fds(MAX_CLIENTS + 1); // +1 for the listening socket
  int nb_active_fds = 0; // Number of active file descriptors

  // Add the listening socket to the pollfd array
  array_fds[0].fd = server_socket;
  array_fds[0].events = POLLIN; // Monitor for incoming data (new connections)
  nb_active_fds++;

  // Main polling loop
  while (true) {
    int sum_updated_fds = poll(array_fds.data(), nb_active_fds, -1); // -1 for infinite timeout
    if (sum_updated_fds < 0) {
      std::cerr << "Error in poll()" << std::endl;
      break;
    }

    // Check if the listening/server socket has an event (new connection)
    if (array_fds[0].revents & POLLIN) {
      sockaddr_in client_addr;
      socklen_t client_len = sizeof(client_addr);
      int client_sock = accept(server_socket, (sockaddr*)&client_addr, &client_len);
      if (client_sock < 0)
        std::cerr << "Error accepting connection" << std::endl;
      else {
        if (nb_active_fds < MAX_CLIENTS + 1) {
          array_fds[nb_active_fds].fd = client_sock;
          array_fds[nb_active_fds].events = POLLIN; // Monitor for incoming data from client
          nb_active_fds++;
          std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr)
                    << ":" << ntohs(client_addr.sin_port) << std::endl;
        } else {
          std::cerr << "Max clients reached, closing new connection" << std::endl;
          close(client_sock);
        }
      }
      sum_updated_fds--; // Decrement count as we handled this (server's) event
    }

    // Check for events on client sockets
    for (int i = 1; ((i < nb_active_fds) && (sum_updated_fds > 0)); ++i) {
      if (array_fds[i].revents & POLLIN) {
        char buffer[1024];
        ssize_t bytes_read = recv(array_fds[i].fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_read <= 0) { // Connection closed or error

          if (bytes_read == 0)
            std::cout << "Client disconnected: " << array_fds[i].fd << std::endl;
          else
            std::cerr << "Error reading from client: " << array_fds[i].fd << std::endl;
          close(array_fds[i].fd);
          // Remove the disconnected client from the array
          for (int j = i; j < nb_active_fds - 1; ++j)
            array_fds[j] = array_fds[j + 1];
          nb_active_fds--;
          i--; // Re-check the current index as it now contains a new fd
        } else {
          buffer[bytes_read] = '\0';
          std::cout << "Received from client " << array_fds[i].fd << ": " << buffer << std::endl;
          // Echo back the received data
          send(array_fds[i].fd, buffer, bytes_read, 0);
        }
        sum_updated_fds--;
      }
    }
  }

  // Close all open sockets (not strictly necessary in this infinite loop example, but good practice)
  for (int i = 0; i < nb_active_fds; ++i)
    close(array_fds[i].fd);

  return 0;
}

// eof
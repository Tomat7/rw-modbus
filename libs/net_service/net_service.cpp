// telnet.cpp

#include <iostream>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>
#include <cstring> // For memset

#include "net_service.h"

int NetService_c::_init_socket()
{
  // 1. Create a listening socket
  server_socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (server_socket_ < 0) {
    std::cerr << "Error creating socket" << std::endl;
    return 1;
  }

  return 0;
}

int NetService_c::_set_socket()
{
  // 2. Set socket options to reuse address (??)
  int opt = 1;
  if (setsockopt(server_socket_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
    std::cerr << "Error setting socket options" << std::endl;
    close(server_socket_);
    return 1;
  }

  return 0;
}


int NetService_c::_bind_socket()
{
  // 3. Bind the listening socket to an address and port
  sockaddr_in server_addr;
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(port_);

  if (bind(server_socket_, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
    std::cerr << "Error binding socket" << std::endl;
    close(server_socket_);
    return 1;
  }

  return 0;
}

int NetService_c::_listen_socket()
{
  // Listen for incoming connections
  if (listen(server_socket_, 5) < 0) {
    std::cerr << "Error listening on socket" << std::endl;
    close(server_socket_);
    return 1;
  }

  return 0;
}


int NetService_c::init(uint16_t _p, int _maxcl)
{
  if (_p)
    port_ = _p;
  if (_maxcl)
    max_clients_ = _maxcl;

  active_fds.resize(max_clients_ + 1);
  _init_socket();
  _set_socket();
  _bind_socket();
  _listen_socket();

  std::cout << "Server listening on port: " << port_
            << ", socket: " << server_socket_ << std::endl;

  return 0;
}


int NetService_c::run()
{
  // Reset Number of active file descriptors
  nb_active_fds_ = 0;

  // Add the listening socket to the pollfd array
  active_fds[0].fd = server_socket_;
  active_fds[0].events = POLLIN; // Monitor for incoming data (new connections)
  nb_active_fds_++;

  std::cout << "Server running: " << active_fds.size() << " "
            << active_fds.capacity() << std::endl;

// Main polling loop
  while (true) {                           // -1 for infinite timeout
    int sum_updated_fds = poll(active_fds.data(), nb_active_fds_, -1);
    if (sum_updated_fds < 0) {
      std::cerr << "Error in poll()" << std::endl;
      break;
    }

    // Check if the listening/server socket has an event (new connection)
    if (active_fds[0].revents & POLLIN) {
      _new_client();
      sum_updated_fds--; // Decrement count as we handled this (server's) event
    }

    // Check for events on client sockets
    for (int i = 1; ((i < nb_active_fds_) && (sum_updated_fds > 0)); ++i) {
      if (active_fds[i].revents & POLLIN) {
        ssize_t bytes_read = recv(active_fds[i].fd, buffer, sizeof(buffer) - 1, 0);

        if (bytes_read <= 0) { // Connection closed or error
          _bad_client(i);
          i--; // Re-check the current index as it now contains a new fd
        } else
          _echo_client(i, bytes_read);

        sum_updated_fds--;
      }
    }
  }

  return 0;
}

int NetService_c::_echo_client(int i, ssize_t bytes_read)
{
  buffer[bytes_read] = '\0';
  std::cout << "Received from client " << active_fds[i].fd << ": " << buffer << std::endl;
  // Echo back the received data
  // send(active_fds[i].fd, buffer, bytes_read, 0);
  send(active_fds[i].fd, answer_.c_str(), answer_.length(), 0);
  close(active_fds[i].fd);

  return 0;
}

int NetService_c::_answer_client(int fd_)
{
  send(fd_, answer_.c_str(), answer_.length(), 0);
  return 0;
}

int NetService_c::_new_client()
{
  int rc = 0;
  sockaddr_in client_addr;
  socklen_t client_len = sizeof(client_addr);
  int client_sock = accept(server_socket_, (sockaddr*)&client_addr, &client_len);

  if (client_sock < 0)
    std::cerr << "Error accepting connection" << std::endl;
  else {
    if (nb_active_fds_ < max_clients_ + 1) {
      active_fds[nb_active_fds_].fd = client_sock;
      active_fds[nb_active_fds_].events = POLLIN; // Monitor for incoming data from client
      nb_active_fds_++;
      rc = 1;
      //_answer_client(client_sock);
      std::cout << "New connection from " << inet_ntoa(client_addr.sin_addr)
                << ":" << ntohs(client_addr.sin_port) << std::endl;
    } else {
      std::cerr << "Max clients reached, closing new connection" << std::endl;
      close(client_sock);
    }
  }

  return rc;
}

int NetService_c::_bad_client(int i)
{
  std::cout << "Client error/disconnected: " << active_fds[i].fd << std::endl;
  close(active_fds[i].fd);

  // Remove the disconnected client from the array
  for (int j = i; j < nb_active_fds_ - 1; ++j)
    active_fds[j] = active_fds[j + 1];

  nb_active_fds_--;

  return 0;
}

void NetService_c::set_answer(string s)
{
  answer_ = s;
}


int NetService_c::stop()
{
  // Close all open sockets (good practice)
  for (int i = 0; i < nb_active_fds_; ++i)
    close(active_fds[i].fd);
  active_fds.clear();
  return 0;
}

// eof
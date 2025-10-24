// telnet.cpp

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <poll.h>

#include <iostream>
#include <vector>
#include <mutex>

#include <cstring> // For memset

#define PORT_ 12345
#define MAX_CLIENTS_ 10

using std::string;
using std::mutex;

class TelnetSvc_c
{
public:
  // Initialize pollfd structures (+1 for listening socket)
  TelnetSvc_c(uint16_t _p = PORT_, int _maxcl = MAX_CLIENTS_)
    : port_(_p), max_clients_(_maxcl) {};

  ~TelnetSvc_c() { stop(); };

  int init();
  int run();
  int stop();

private:
  int _init_socket();
  int _set_socket();
  int _bind_socket();
  int _listen_socket();

  int _new_client();
  int _bad_client(int);
  int _handle_client(int, ssize_t);
//  int _handle_socket();

  uint16_t port_ = PORT_;
  int max_clients_ = MAX_CLIENTS_;
  int server_socket_;
  int nb_active_fds_ = 0; // Number of active file descriptors
  char buffer[1024];
  std::vector<pollfd> active_fds;

};

// eof

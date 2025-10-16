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
  TelnetSvc_c(int _p = PORT_, int _maxcl = MAX_CLIENTS_)
    : port_(_p), max_clients_(_maxcl) {};

  int init();

private:
  int port_ = PORT_;
  int max_clients_ = MAX_CLIENTS_;
  int server_socket;
  int nb_active_fds = 0; // Number of active file descriptors
  int nb_updated_fds;

  sockaddr_in server_addr;

  std::vector<pollfd> array_fds; //(MAX_CLIENTS_ + 1); // +1 for the listening socket


};

// eof
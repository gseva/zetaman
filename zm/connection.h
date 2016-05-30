#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>

#include "zm/thread.h"


#define NUM_CLIENTS 5

#ifndef MSG_NOSIGNAL
#define SOCKET_FLAGS 0
#else
#define SOCKET_FLAGS MSG_NOSIGNAL
#endif

namespace zm {

class Socket {
  int fd_;

  explicit Socket(int fd);

public:
  Socket();

  int connect(const std::string& hostname, const std::string& port);

  int write(const std::string& s);
  std::string readLine();

  int bind(const std::string& port);
  Socket accept();
  int listen();
  int bindAndListen(const std::string& port);

  int close();

  ~Socket();
};


class Accepter : public Thread {
Mutex m_;
Socket sock_;

};

}// zm
#endif

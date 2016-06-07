#ifndef __CONNECTION_H__
#define __CONNECTION_H__

#include <string>
#include <memory>

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

  int writeData_(const char* data, size_t bytes);
  int receiveData_(char* result, size_t bytes);
public:
  Socket();
  explicit Socket(int fd);

  int connect(const std::string& hostname, const std::string& port);

  int write(const std::string& s);
  std::string read();

  int bind(const std::string& port);
  std::shared_ptr<Socket> accept();
  int listen();
  int bindAndListen(const std::string& port);

  int shutdown();
  int close();

  ~Socket();
};

class ProtectedSocket {
  Mutex writeMutex;
  Mutex readMutex;

  std::shared_ptr<Socket> s_;
public:
  ProtectedSocket();
  ProtectedSocket(std::shared_ptr<Socket> s);
  int write(const std::string& s);
  std::string read();

  int connect(const std::string& hostname, const std::string& port);
  void close();
};


}// zm
#endif

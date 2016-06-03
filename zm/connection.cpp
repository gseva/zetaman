
#include <exception>
#include <string>
#include <string.h>
#include <strings.h>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <netinet/in.h>

#include <iostream>

#include "zm/connection.h"

#define MAX_BUFF_SIZE 4096

namespace zm {


Socket::Socket() {
  fd_ = socket(AF_INET, SOCK_STREAM, 0);
}

Socket::Socket(int fd) : fd_(fd) {
}

int Socket::connect(const std::string& hostname, const std::string& port) {
  struct addrinfo hints, *servinfo, *p;

  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  getaddrinfo(hostname.c_str(), port.c_str(), &hints, &servinfo);

  for (p = servinfo; p != NULL; p = p->ai_next) {
    if (::connect(fd_, p->ai_addr, p->ai_addrlen) != -1) {
      break;
    }
  }

  freeaddrinfo(servinfo);
  return 0;
}

std::shared_ptr<Socket> Socket::accept() {
  struct sockaddr_in cli_addr;
  socklen_t cli_len = sizeof(cli_addr);
  int newfd = ::accept(fd_, (struct sockaddr *) &cli_addr, &cli_len);
  return std::make_shared<Socket> (newfd);;
}


int Socket::bind(const std::string& port) {
  std::stringstream ss(port);
  int p;
  ss >> p;
  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(p);

  return ::bind(fd_, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
}

int Socket::listen() {
  return ::listen(fd_, NUM_CLIENTS);
}


int Socket::bindAndListen(const std::string& port) {
  if (0 < bind(port)) {
    throw std::runtime_error("No pude bindear el puerto " + port);
  }
  return listen();
}

int Socket::writeData_(const char* data, size_t bytes) {
  while (bytes > 0)
  {
    ssize_t i = send(fd_, data, bytes, SOCKET_FLAGS);
    if (i < 1) return i;
    data += i;
    bytes -= i;
  }
  return 0;
}

int Socket::receiveData_(char* result, size_t bytes) {
  while (bytes > 0) {
    ssize_t i = recv(fd_, result, bytes, SOCKET_FLAGS);
    if (i < 1) {
      return i;
    }
    bytes -= i;
    result += i;
  }
  return 0;
}

int Socket::write(const std::string& s) {
  int stringLength = s.length();
  uint32_t length = htonl(stringLength);
  int res = writeData_((char*) &length, sizeof(uint32_t));
  if (res != 0) return res;
  const char* data = s.c_str();
  return writeData_(data, stringLength);
}

std::string Socket::readLine() {
  uint32_t length;
  if (receiveData_((char*) &length, sizeof(uint32_t)) != 0) {
    throw std::runtime_error("Error leyendo del socket!");
  }
  length = ntohl(length);
  std::string result(length, 0);
  if (receiveData_(&(result[0]), length) != 0) {
    throw std::runtime_error("Error leyendo del socket!");
  }
  return result;
}


int Socket::close() {
  ::shutdown(fd_, SHUT_RDWR);
  return ::close(fd_);
}

Socket::~Socket() {
  close();
}


} // zm

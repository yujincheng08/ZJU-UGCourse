//
// Created by lovesy on 18-5-8.
//

#include <exception>
#include <cstring>
#include <netdb.h>
#include <iostream>
#include "socket.h"
extern "C" {
#ifdef _WIN32
#include <winsock.h>
#endif
#ifdef __unix
#include <unistd.h>
#endif
};

ServerSocket::ServerSocket() = default;

void ServerSocket::bind(std::string const &ip, std::uint16_t const &port, const unsigned int &backlog) {
  addrinfo hints{};
  hints.ai_family = PF_INET6;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;
  hints.ai_protocol = 0;

  const char *host = ip.empty() ? nullptr : ip.c_str();

  addrinfo *info;
  if (auto s = getaddrinfo(host, std::to_string(port).c_str(), &hints, &info) < 0)
    throw SocketException("Resolve error: " + std::string(gai_strerror(s)));
  for (auto address = info; address != nullptr; address = address->ai_next) {
    socket_ = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (socket_ < 0)
      if (address->ai_next == nullptr)
        continue;
      else
        throw SocketException("Create socket error: " + std::string(strerror(errno)));

    if (::bind(socket_, address->ai_addr, address->ai_addrlen) == 0)
      break;
    else if (address->ai_next == nullptr)
      throw SocketException("Bind error: " + std::string(strerror(errno)));
    if (::close(socket_) < 0)
      throw SocketException("Close socket error: " + std::string(strerror(errno)));
  }
  freeaddrinfo(info);
  if (::listen(socket_, backlog) < 0)
    throw SocketException("Listen error: " + std::string(strerror(errno)));
  isClose_ = false;
}

Socket ServerSocket::accept() {
  Socket result;
  sockaddr_storage address{};
  socklen_t addressLen = sizeof(sockaddr_storage);
  result.socket_ = ::accept(socket_, reinterpret_cast<sockaddr *> (&address), &addressLen);
  if (result.socket_ < 0)
    throw SocketException("Accept Error: " + std::string(strerror(errno)));
  char host[NI_MAXHOST], service[NI_MAXSERV];
  if (auto s = getnameinfo(reinterpret_cast<sockaddr *>(&address), addressLen, host, NI_MAXHOST,
                           service, NI_MAXSERV, NI_NUMERICSERV) < 0)
    throw SocketException("Resolve error: " + std::string(gai_strerror(s)));
  std::cout << "Receive from: " << host << ':' << service << std::endl;
  return result;
}

void ServerSocket::close() {
  ::close(socket_);
  isClose_ = true;
}
ServerSocket::ServerSocket(std::uint16_t const &port, unsigned const &backlog, std::string const &ip)
    : ServerSocket() {
  bind(ip, port, backlog);
}

ServerSocket::~ServerSocket() {
  if (!isClose_)
    close();
}

Socket::Socket(std::string const &hostname, std::uint16_t const &port)
    : Socket() {
  connect(hostname, port);
}

void Socket::connect(std::string const &hostname, std::uint16_t const &port) {
  addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  addrinfo *info;
  if (auto s = getaddrinfo(hostname.c_str(), std::to_string(port).c_str(), &hints, &info) < 0)
    throw SocketException("Hostname resolve error:" + std::string(gai_strerror(s)));

  for (auto address = info; address != nullptr; address = address->ai_next) {
    socket_ = socket(address->ai_family, address->ai_socktype, address->ai_protocol);
    if (socket_ < 0)
      if (address->ai_next == nullptr)
        continue;
      else
        throw SocketException("Create socket error: " + std::string(strerror(errno)));

    if (::connect(socket_, address->ai_addr, address->ai_addrlen) == 0)
      break;
    else if (address->ai_next == nullptr)
      throw SocketException("Error connect: " + std::string(strerror(errno)));

    if (::close(socket_) < 0)
      throw SocketException("Close socket error: " + std::string(strerror(errno)));
  }
  freeaddrinfo(info);
}

Socket::Socket() = default;

std::vector<unsigned char> Socket::read(size_t const &size) {
  std::vector<unsigned char> result(size);
  auto realSize = ::read(socket_, &result[0], size);
  if (realSize < 0)
    throw SocketException("Read error: " + std::string(strerror(errno)));
  else
    result.resize(static_cast<size_t>(realSize));
  return result;

}

ssize_t Socket::write(std::vector<unsigned char> const &buff) {
  auto result = ::write(socket_, &buff[0], buff.size());
  if (result < 0)
    throw SocketException("Write error: " + std::string(strerror(errno)));
  return result;
}

void Socket::close() {
  if (::close(socket_) < 0)
    throw SocketException("Close error: " + std::string(strerror(errno)));
}


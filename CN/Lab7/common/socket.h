//
// Created by lovesy on 18-5-8.
//

#ifndef COMMON_SOCKET_H
#define COMMON_SOCKET_H

#include <string>

#include <exception>
#include <vector>

class SocketException : public std::exception {
 public:
  explicit SocketException(const std::string &what) {
    what_ = what;
  }
  char const *what() const noexcept override {
    return what_.c_str();
  }
  std::string what_;
};

class Socket;

class ServerSocket {
  static constexpr unsigned DEFAULT_BACKLOG = 50u;
 public:
  explicit ServerSocket();
  explicit ServerSocket(std::string const &port,
                        unsigned const &backlog = DEFAULT_BACKLOG,
                        std::string const &ip = std::string());
  void bind(std::string const &ip,
            std::string const &port,
            unsigned const &backlog = DEFAULT_BACKLOG);
  Socket accept();
  void close();
  inline bool isClose() const { return isClose_; }
  ~ServerSocket();

 private:
  int socket_{0};
  bool isClose_{true};
};

class Socket {
  friend class ServerSocket;
 public:
  explicit Socket();
  explicit Socket(std::string const &hostname, std::string const &port);
  ssize_t write(std::string const &buff);
  void close();
  std::string read(size_t const &size);
  void connect(std::string const &hostname, std::string const &port);
  std::string host() const { return host_; }
  std::string service() const { return service_; }
  bool isClosed() const { return isClosed_; }

 private:
  bool isClosed_{true};
  int socket_{0};
  std::string host_{};
  std::string service_{};
};

#endif //COMMON_SOCKET_H

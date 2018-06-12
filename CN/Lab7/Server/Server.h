//
// Created by lovesy on 18-5-31.
//

#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <socket.h>
#include <unordered_map>
#include <mutex>
#include <list>
#include <thread>

class Server {
 public:
  Server(int argc, char* argv[]);

  void start();

  ~Server();

 private:
  ServerSocket serverSocket_;

  void accept();

  void handle(Socket& socket);

  void handleRequire(Socket &socket, Require const &require, ClientMessage const &client);

  void log(uint64_t const &id, std::string const &msg) ;

  std::unordered_map<std::uint64_t, Socket> clientSockets_;

  std::list<ClientMessage> clients_;

  std::mutex mutex_;

  std::uint64_t id_{0};

  std::thread acceptThread_;

  std::list<std::thread> threads_;

};

#endif //SERVER_SERVER_H

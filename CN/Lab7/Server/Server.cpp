//
// Created by lovesy on 18-5-31.
//


#include <thread>
#include <functional>
#include <message.h>
#include <sstream>
#include "Server.h"
#include <ulimit.h>
#include <zconf.h>

Server::Server(int argc, char **argv) {
}

void Server::handle(Socket &socket) {
  mutex_.lock();
  auto id = ++id_;
  clientSockets_[id] = socket;
  ClientMessage clientMessage(id, socket.host(), socket.service());
  auto client = clients_.insert(clients_.end(), clientMessage);
  mutex_.unlock();
  log(id, "connected");
  Reply reply(HELLO);
  reply.message("hello Client #" + std::to_string(id));
  reply.sendTo(socket);
  while (true) {
    try {
      Require require;
      require.readFrom(socket);
      handleRequire(socket, require, *client);
    } catch (SocketException &e) {
      std::cerr << "Client #" << id << ": " << e.what() << std::endl;
      if (!socket.isClosed())
        continue;
      {
        std::lock_guard<std::mutex> lk(mutex_);
        clientSockets_.erase(id);
        clients_.erase(client);
      }
      break;
    }
  }

}
void Server::handleRequire(Socket &socket, Require const &require, ClientMessage const &client) {
  switch (require.type()) {
    case TIME: {
      log(client.id(), "requires time.");
      Reply reply(TIME);
      std::time_t result = std::time(nullptr);
      reply.timestamp(result);
      reply.sendTo(socket);
      break;
    }
    case NAME: {
      log(client.id(), "requires name.");
      Reply reply(NAME);
      char hostname[HOST_NAME_MAX];
      gethostname(hostname, HOST_NAME_MAX);
      reply.name(std::string(hostname));
      reply.sendTo(socket);
      break;
    }
    case CLIENT: {
      log(client.id(), "requires clients.");
      Reply reply(CLIENT);
      reply.clients().assign(clients_.begin(), clients_.end());
      reply.sendTo(socket);
      break;
    }
    case SEND: {
      log(client.id(), "send messages.");
      if (require.hasId() && require.hasMessage()) {
        auto id = require.id();
        auto clientSocket = clientSockets_.find(id);
        if (clientSocket == clientSockets_.end()) {
          Reply reply(ERROR);
          reply.message("Not a valid ID");
          reply.sendTo(socket);
        } else {
          try {
            Reply msg(RECEIVE);
            msg.client(client);
            msg.message(require.message());
            msg.sendTo(clientSocket->second);
          } catch (SocketException &e) {
            Reply reply(ERROR);
            reply.message(e.what());
            reply.sendTo(socket);
            break;
          }
          Reply reply(SEND);
          reply.message("Sent");
          reply.sendTo(socket);
        }
        break;
      } else {
        Reply reply(ERROR);
        reply.message("Not a valid send message");
        reply.sendTo(socket);
      }
    }
    default: {
      Reply reply(ERROR);
      reply.message("Not a valid message");
      reply.sendTo(socket);
      break;
    }
  }
}

Server::~Server() = default;

void Server::accept() {
  while (true) {
    try {
      auto socket = serverSocket_.accept();
      threads_.emplace_back(std::bind(&Server::handle, this, socket));
    } catch (SocketException &e) {
      std::cerr << e.what() << std::endl;
      break;
    }
  }
}
void Server::start() {
  std::string command;
  const std::string host = "::";
  const std::string port = "1155";
  try {
    serverSocket_.bind(host, port);
  } catch (SocketException &e) {
    std::cerr << e.what() << std::endl;
    return;
  }
  std::cout << "Server started on " << host << ":" << port << std::endl;
  std::cout << "Press ^C or type quit to stop the server." << std::endl;
  acceptThread_ = std::thread(std::bind(&Server::accept, this));
  while (true) {
    std::cin >> command;
    if (command == "quit") {
      std::cout << "bye" << std::endl;
      break;
    } else
      std::cerr << "Enter quit to end the server" << std::endl;
  }
  serverSocket_.close();
  for (auto socket : clientSockets_)
    if (!socket.second.isClosed())
      socket.second.close();
  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.join();
    }
  }
  if (acceptThread_.joinable()) {
    acceptThread_.join();
  }
  clientSockets_.clear();
  threads_.clear();
}

void Server::log(uint64_t const &id, std::string const &msg) {
  std::lock_guard<std::mutex> lk(mutex_);
  std::cout << "Client #" << id << ": " << msg << std::endl;
}

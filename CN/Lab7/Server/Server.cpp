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
  std::cout << "Client #" << id << " " << socket.host() << ":" << socket.service() << " connected" << std::endl;
  Reply reply(HELLO);
  reply.message("hello Client #" + std::to_string(id));
  reply.sendTo(socket);
  while (true) {
    try {
      Require require;
      require.readFrom(socket);
      if (require.type() == CLOSE) {
        std::cout << "Client #" << id << " disconnect." << std::endl;
        mutex_.lock();
        clientSockets_.erase(id);
        clients_.erase(client);
        mutex_.unlock();
        break;
      }
      handleRequire(socket, require, *client);
    } catch (SocketException &e) {
      std::cerr << "Client #" << id << ": " << e.what() << std::endl;
      mutex_.lock();
      clientSockets_.erase(id);
      clients_.erase(client);
      mutex_.unlock();
      break;
    }
  }

}
void Server::handleRequire(Socket &socket, Require const &require, ClientMessage const &client) {
  switch (require.type()) {
    case TIME: {
      Reply reply(TIME);
      std::time_t result = std::time(nullptr);
      reply.timestamp(result);
      reply.sendTo(socket);
      break;
    }
    case NAME: {
      Reply reply(NAME);
      char hostname[HOST_NAME_MAX];
      gethostname(hostname, HOST_NAME_MAX);
      reply.name(std::string(hostname));
      reply.sendTo(socket);
      break;
    }
    case CLIENT: {
      Reply reply(CLIENT);
      reply.clients().assign(clients_.begin(), clients_.end());
      reply.sendTo(socket);
      break;
    }
    case SEND: {
      if (require.hasId() && require.hasMessage()) {
        auto id = require.id();
        auto clientSocket = clientSockets_.find(id);
        if (clientSocket == clientSockets_.end()) {
          Reply reply(ERROR);
          reply.message("Not a valid ID");
          reply.sendTo(socket);
        } else {
          Reply reply(RECEIVE);
          reply.client(client);
          reply.message(require.message());
          reply.sendTo(clientSocket->second);
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

Server::~Server() {
  mutex_.lock();
  Reply reply(CLOSE);
  for (auto socket : clientSockets_) {
    reply.sendTo(socket.second);
    socket.second.close();
  }
  mutex_.unlock();
}
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
  serverSocket_.bind("::", "6000");
  acceptThread_ = std::thread(std::bind(&Server::accept, this));
  while (true) {
    std::cin >> command;
    if (command == "quit") {
      std::cout << "bye" << std::endl;
      break;
    } else
      std::cerr << "Enter quit to end the server" << std::endl;
  }
  if (acceptThread_.joinable()) {
    acceptThread_.detach();
    pthread_cancel(acceptThread_.native_handle());
  }
  for (auto &thread : threads_) {
    if (thread.joinable()) {
      thread.detach();
      pthread_cancel(thread.native_handle());
    }
  }
  Reply reply(CLOSE);
  for (auto socket : clientSockets_) {
    if (!socket.second.isClosed()) {
      reply.sendTo(socket.second);
      socket.second.close();

    }
  }
}

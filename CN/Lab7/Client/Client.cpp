//
// Created by lovesy on 18-5-31.
//

#include "Client.h"
#include <socket.h>
#include <sstream>
#include <message.h>
#include <thread>
#include <functional>
#include <chrono>
using namespace std::chrono_literals;

Client::Client(int argc, char **argv) {
  std::thread thread;
  std::string command;
  std::cout << "Please enter command: " << std::endl;
  std::cout << "Available commands: " << std::endl;
  std::cout << "\tconnect, disconnect, quit, timestamp, name, clients, send" << std::endl;
  while (true) {
    try {
      std::cout << "> " << std::flush;
      std::cin >> command;
      if (command == "connect") {
        if (!socket_.isClosed()) {
          std::cerr << "You have to disconnect first" << std::endl;
          continue;
        }
        std::cout << "Hostname and service:" << std::endl;
        std::string host;
        std::string service;
        std::cin >> host >> service;
        socket_.connect(host, service);
        thread = std::thread(std::bind(&Client::receive, this));
        wait();
        continue;
      }
      if (command == "disconnect") {
        if (!socket_.isClosed()) {
          Require require(CLOSE);
          require.sendTo(socket_);
          socket_.close();
          if (thread.joinable()) {
            thread.detach();
            pthread_cancel(thread.native_handle());
          }
        }
        std::cout << "Disconnected from server" << std::endl;
        continue;
      }
      if (command == "quit") {
        std::cout << "Bye" << std::endl;
        if (!socket_.isClosed()) {
          Require require(CLOSE);
          require.sendTo(socket_);
          socket_.close();
        }
        if (thread.joinable()) {
          thread.detach();
          pthread_cancel(thread.native_handle());
        }
        break;
      }
      if (socket_.isClosed()) {
        std::cout << "You have to connect first" << std::endl;
        continue;
      }
      if (command == "timestamp") {
        Require require(TIME);
        require.sendTo(socket_);
        wait();
        continue;
      }
      if (command == "name") {
        Require require(NAME);
        require.sendTo(socket_);
        wait();
        continue;
      }
      if (command == "clients") {
        Require require(CLIENT);
        require.sendTo(socket_);
        wait();
        continue;
      }
      if (command == "send") {
        Require require(SEND);
        std::cout << "Please enter id: " << std::endl;
        decltype(require.id()) id;
        std::cin >> id;
        require.id(id);
        std::cout << "Please enter message: " << std::endl;
        std::string message;
        std::cin >> message;
        require.message(message);
        require.sendTo(socket_);
        continue;
      }
      std::cerr << "Not a valid command" << std::endl;
    } catch (SocketException &e) {
      std::cerr << e.what() << std::endl << "> " << std::ends;
    }
  }

}

void Client::receive() {
  while (true) {
    try {
      Reply reply;
      reply.readFrom(socket_);
      handleReply(reply);
      if(reply.type() == CLOSE)
        break;
    } catch (SocketException &e) {
      std::cerr << e.what() << std::endl;
      break;
    }
  }
}
void Client::handleReply(Reply const &reply) {
  switch (reply.type()) {
    case CLOSE: {
      std::cout << "Disconnect from server" << std::endl;
      socket_.close();
      return;
    }
    case SEND: {
      if(reply.hasMessage()) {
        std::cout<< reply.message() << std::endl;
        cv_.notify_one();
      } else {
        std::cerr << "Error message got from server." << std::endl;
      }
      break;
    }
    case TIME: {
      if (reply.hasTimestamp()) {
        std::cout << "Got server timestamp: " << reply.timestamp() << std::endl;
        cv_.notify_one();
      }
      else
        std::cerr << "Error message got from server." << std::endl;
      break;
    }
    case NAME: {
      if (reply.hasName()) {
        std::cout << "Got server name: " << reply.name() << std::endl;
        cv_.notify_one();
      }
      else
        std::cerr << "Error message get from server." << std::endl;
      break;
    }
    case HELLO: {
      if (reply.hasMessage()) {
        std::cout << "Got a greet from server: " << reply.message() << std::endl;
        cv_.notify_one();
      }
      else
        std::cerr << "Get a greet from server without any message" << std::endl;
      break;
    }
    case CLIENT: {
      std::cout << "All clients are listed as follow:" << std::endl;
      for (auto &client : reply.clients())
        std::cout << "\tClient #" << client.id() << " " << client.ip() << ":" << client.service() << std::endl;
      cv_.notify_one();
      break;
    }
    case RECEIVE: {
      if (reply.hasMessage() && reply.clients().size() == 1) {
        auto client = reply.clients().at(0);
        std::cout << "Got a message from client #"
                  << client.id()
                  << " "
                  << client.ip()
                  << ":"
                  << client.service()
                  << std::endl;
        std::cout << "\t" << reply.message() << std::endl;
        cv_.notify_one();
      } else
        std::cerr << "Error message get from server." << std::endl;
      break;
    }
    case ERROR: {
      std::cerr << "Receive an error from server." << std::endl;
      if (reply.hasMessage())
        std::cerr << "\t" << reply.message() << std::endl;
      cv_.notify_one();
      break;
    }
    default: {
      std::cerr << "Receive an error message." << std::endl;
      break;
    }
  }
}
void Client::wait() {
  std::unique_lock<std::mutex> lock(mutex_);
  cv_.wait_for(lock, 5s);
}


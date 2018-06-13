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
  std::string command;
  std::cout << "Please enter command: " << std::endl;
  printHelp();
  while (true) {
    try {
      std::cout << "> " << std::flush;
      std::cin >> command;
      handleCommand(command);
      if(command == "quit")
        break;
    } catch (SocketException &e) {
      std::cerr << e.what() << std::endl;
      if (socket_.isClosed() && thread_.joinable())
        thread_.join();
    }
  }
}


void Client::receive() {
  while (true) {
    try {
      Reply reply;
      reply.readFrom(socket_);
      handleReply(reply);
    } catch (SocketException &e) {
      std::cerr << e.what() << std::endl;
      cv_.notify_one();
      break;
    }
  }
}
void Client::handleReply(Reply const &reply) {
  switch (reply.type()) {
    case SEND: {
      if (reply.hasMessage()) {
        std::cout << reply.message() << std::endl;
        cv_.notify_one();
      } else {
        std::cerr << "Error message got from server." << std::endl;
      }
      break;
    }
    case TIME: {
      if (reply.hasTimestamp()) {
        static size_t count = 0u;
        std::cout << ++count << ": ";
        std::cout << "Got server timestamp: " << reply.timestamp() << std::endl;
        cv_.notify_one();
      } else
        std::cerr << "Error message got from server." << std::endl;
      break;
    }
    case NAME: {
      if (reply.hasName()) {
        std::cout << "Got server name: " << reply.name() << std::endl;
        cv_.notify_one();
      } else
        std::cerr << "Error message get from server." << std::endl;
      break;
    }
    case HELLO: {
      if (reply.hasMessage()) {
        std::cout << "Got a greet from server: " << reply.message() << std::endl;
        cv_.notify_one();
      } else
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
        std::cout << reply.message() << std::endl;
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
  if (cv_.wait_for(lock, 5s) == std::cv_status::timeout) {
    std::cerr << "Timeout waiting for server" << std::endl;
  }
}

void Client::printHelp() const {
  std::cout << "Available commands: " << std::endl;
  std::cout << "\tconnect host port:   connect to host:port," << std::endl
            << "\tdisconnect:          disconnect from server," << std::endl
            << "\tquit:                quit the program," << std::endl
            << "\ttimestamp:           get timestamp from server," << std::endl
            << "\tname:                get the name from server," << std::endl
            << "\tclients:             get clients list from server," << std::endl
            << "\tsend client message: send message to client, message ends with ^D (EOF)," << std::endl
            << "\thelp:                print this message." << std::endl;
}
void Client::handleCommand(std::string const &command) {
  if (command == "help") {
    printHelp();
    return;
  }
  if (command == "connect") {
    if (!socket_.isClosed()) {
      std::cerr << "You have to disconnect first" << std::endl;
      return;
    }
    std::string host;
    std::string service;
    std::cin >> host >> service;
    socket_.connect(host, service);
    thread_ = std::thread(std::bind(&Client::receive, this));
    wait();
    return;
  }
  if (command == "disconnect") {
    if (!socket_.isClosed())
      socket_.close();
    if(thread_.joinable())
      thread_.join();
    return;
  }
  if (command == "quit") {
    std::cout << "Bye" << std::endl;
    if (!socket_.isClosed())
      socket_.close();
    if(thread_.joinable())
      thread_.join();
    return;
  }
  if (socket_.isClosed()) {
    std::cout << "You have to connect first" << std::endl;
    return;
  }
  if (command == "timestamp") {
    Require require(TIME);
    for(size_t i = 0u; i < 100u; ++i)
      require.sendTo(socket_);
    wait();
    return;
  }
  if (command == "name") {
    Require require(NAME);
    require.sendTo(socket_);
    wait();
    return;
  }
  if (command == "clients") {
    Require require(CLIENT);
    require.sendTo(socket_);
    wait();
    return;
  }
  if (command == "send") {
    Require require(SEND);
    decltype(require.id()) id;
    std::cin >> id;
    require.id(id);

    std::string first;
    std::cin >> first;
    std::stringstream message;

    message << first;
    std::copy(std::istreambuf_iterator<char>(std::cin),
              std::istreambuf_iterator<char>(),
              std::ostreambuf_iterator<char>(message)
    );
    std::cin.clear();
    require.message(message.str());
    require.sendTo(socket_);
    wait();
    return;
  }
  std::cerr << "Not a valid command" << std::endl;
}


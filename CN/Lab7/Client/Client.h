//
// Created by lovesy on 18-5-31.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <message.h>
#include <mutex>
#include <condition_variable>
#include <thread>

class Client {
 public:
  Client(int argc, char *argv[]);
 private:
  void receive();
  void handleReply(Reply const &reply);
  void handleCommand(std::string const &command);
  void wait();
  void printHelp() const;
  Socket socket_;
  std::mutex mutex_;
  std::condition_variable cv_;
  std::thread thread_;
};

#endif //SERVER_CLIENT_H

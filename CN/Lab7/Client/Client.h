//
// Created by lovesy on 18-5-31.
//

#ifndef SERVER_CLIENT_H
#define SERVER_CLIENT_H

#include <message.h>
#include <mutex>
class Client {
 public:
  Client(int argc, char *argv[]);
 private:
  void receive();
  void handleReply(Reply const &reply);
  Socket socket_;
};

#endif //SERVER_CLIENT_H

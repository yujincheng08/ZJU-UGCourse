//
// Created by lovesy on 18-4-22.
//

#include <iostream>
#include "message.h"
#include <sstream>

using namespace std;

int main(int argc, char *argv[]) {
  Reply reply(RECIEVE);
  stringstream ss;
  ss << reply;
  Reply newReply;
  ss >> newReply;
  cout << newReply.type() << newReply.hasTimestamp() << newReply.hasName() << newReply.hasMessage() << endl;
  newReply.message("hello");
  ss << reply;
  ss >> newReply;
  cout << newReply.type() << newReply.hasTimestamp() << newReply.hasName() << newReply.hasMessage() << endl;
  cout << newReply.message() << endl;
  newReply.name("bye");
  ss << reply;
  ss >> newReply;
  cout << newReply.type() << newReply.hasTimestamp() << newReply.hasName() << newReply.hasMessage() << endl;
  cout << newReply.name() << endl;
  newReply.timestamp(1234u);
  ss << reply;
  ss >> newReply;
  cout << newReply.type() << newReply.hasTimestamp() << newReply.hasName() << newReply.hasMessage() << endl;
  cout << newReply.timestamp() << endl;
  reply.client(ClientMessage(12, "::", 30));
  reply.client(ClientMessage(80, "localhost", 20));
  ss << reply;
  ss >> newReply;
  for (auto const &client : newReply.clients()) {
    cout << client.id() << client.ip() << client.service() << endl;
  }
}


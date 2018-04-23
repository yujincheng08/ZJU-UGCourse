//
// Created by lovesy on 18-4-22.
//

#include <iostream>
#include <sstream>
#include <boost/asio.hpp>

#include "message.h"

using namespace std;
using namespace boost::asio;

int main() {
  ip::tcp::iostream stream;
  stream.expires_from_now(boost::posix_time::seconds(60));
  stream.connect("www.boost.org", "http");
  stream << "GET /LICENSE_1_0.txt HTTP/1.0\r\n";
  stream << "Host: www.boost.org\r\n";
  stream << "Accept: */*\r\n";
  stream << "Connection: close\r\n\r\n";
  stream.flush();
  Client newClient;
  stream >> newClient;

  Client client(233u, "::", 666u);
  cout << client.id() << client.ip() << client.port() << endl;
  stringstream ss;
  ss << client;
  ss >> newClient;
  cout << newClient.id() << newClient.ip() << newClient.port();
  return 0;
}


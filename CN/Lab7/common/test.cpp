//
// Created by lovesy on 18-4-22.
//

#include <iostream>
#include <cstring>
#include "socket.h"

using namespace std;

int main(int argc, char *argv[]) {
  std::uint16_t port = 23333u;
  if (argc < 2) {
    std::cerr << "No enough options" << std::endl;
    return 2;
  }
  if (argv[1] == std::string("server")) {
    try {
      ServerSocket serverSocket(port);
      while (true) {
        auto socket = serverSocket.accept();
        size_t size;
        auto serializeSize = socket.read(sizeof(size));
        std::memcpy(&size, &serializeSize[0], sizeof(size));
        cout << size << endl;
        auto toRead = socket.read(size);
        for (auto const &i : toRead)
          cout << i;
        cout << endl;
        socket.close();
      }
    } catch (SocketException const &e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  } else if (argv[1] == std::string("client")) {
    try {
      Socket socket("localhost", port);
      string sendString("Hello World!");
      auto size = sendString.size();
      auto serializeSize = reinterpret_cast<unsigned char *>(&size);
      socket.write({serializeSize, serializeSize + sizeof(size)});
      socket.write(sendString);
    } catch (SocketException const &e) {
      std::cerr << e.what() << std::endl;
      return 1;
    }
  }
}


//
// Created by lovesy on 18-4-22.
//

#ifndef COMMON_MESSAGE_H
#define COMMON_MESSAGE_H

#include <vector>
#include <queue>
#include <ostream>
#include <istream>

enum Type : char {
  TIME = 0,
  NAME = 1,
  CLIENT = 2,
  SEND = 3,
  RECIEVE = 4,
  ERROR = 5,
};

class Message {
 protected:
  virtual std::ostream &serialize(std::ostream &buff) const = 0;
  virtual std::istream &deserialize(std::istream &buff) = 0;
  template<class T>
  friend
  class MessageArray;
  friend std::ostream &operator<<(std::ostream &stream, const Message &message);
  friend std::istream &operator>>(std::istream &stream, Message &message);
};

template<class T>
class MessageArray : public std::enable_if_t<std::is_base_of_v<Message, T>, Message> {
 protected:
  std::ostream &serialize(std::ostream &buff) const override;
  std::istream &deserialize(std::istream &buff) override;
 private:
  std::vector<T> array_;
};

template<class T>
std::istream &MessageArray<T>::deserialize(std::istream &buff) {
  size_t size;
  buff.read(reinterpret_cast<char *>(&size), sizeof(size));
  array_.resize(size);
  for (auto &element : array_)
    element.deserialize(buff);
  return buff;
}

#endif //COMMON_MESSAGE_H

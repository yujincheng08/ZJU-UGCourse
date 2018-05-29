#include "message.h"
std::ostream &operator<<(std::ostream &stream, const Message &message) {
  return message.serialize(stream);
}

std::istream &operator>>(std::istream &stream, Message &message) {
  return message.deserialize(stream);
}

template<>
std::ostream &MessageArray<Message>::serialize(std::ostream &buff) const {
  auto size = array_.size();
  buff.write(reinterpret_cast<char *>(size), sizeof(size));
  for (auto const &element : array_)
    element.serialize(buff);
  return buff;
}
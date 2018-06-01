#include <iostream>
#include <sstream>
#include "message.h"
std::ostream &operator<<(std::ostream &stream, const Message &message) {
  return message.serialize(stream);
}

std::istream &operator>>(std::istream &stream, Message &message) {
  return message.deserialize(stream);
}
void Message::sendTo(Socket &socket) const {
  std::stringstream ss;
  ss << *this;
  auto buff = ss.str();
  auto size = buff.size();
  ss = std::stringstream();
  ss.write(reinterpret_cast<char const *>(&size), sizeof(size));
  socket.write(ss.str());
  socket.write(buff);
}
void Message::readFrom(Socket &socket) {
  std::string buff;
  decltype(buff.size()) size;
  buff = socket.read(sizeof(size));
  std::stringstream ss = std::stringstream(buff);
  ss.read(reinterpret_cast<char *>(&size), sizeof(size));
  buff = socket.read(size);
  ss = std::stringstream(buff);
  ss >> *this;
}

std::ostream &ClientMessage::serialize(std::ostream &buff) const {
  buff.write(reinterpret_cast<char const *>(&id_), sizeof(id_));
  auto size = ip_.size();
  buff.write(reinterpret_cast<char const *>(&size), sizeof(size));
  buff << ip_;
  size = service_.size();
  buff.write(reinterpret_cast<char const *>(&size), sizeof(size));
  buff << service_;
  return buff;
}

std::istream &ClientMessage::deserialize(std::istream &buff) {
  buff.read(reinterpret_cast<char *>(&id_), sizeof(id_));
  decltype(ip_.size()) size = 0;
  buff.read(reinterpret_cast<char *>(&size), sizeof(size));
  ip_.resize(size);
  if(size > 0)
    buff.read(&ip_[0], size);
  buff.read(reinterpret_cast<char *>(&size), sizeof(size));
  service_.resize(size);
  if(size > 0)
    buff.read(&service_[0], size);
  return buff;
}

std::ostream &Require::serialize(std::ostream &buff) const {
  buff.write(reinterpret_cast<char const *>(&type_), sizeof(type_));
  auto hasId_ = hasId();
  buff.write(reinterpret_cast<char const *>(&hasId_), sizeof(hasId_));
  if (hasId_)
    buff.write(reinterpret_cast<char const *>(id_.get()), sizeof(*id_));
  auto hasMessage_ = hasMessage();
  buff.write(reinterpret_cast<char const *>(&hasMessage_), sizeof(hasMessage_));
  if (hasMessage_) {
    auto size = message_->size();
    buff.write(reinterpret_cast<char const *>(&size), sizeof(size));
    buff << *message_;
  }
  return buff;
}

std::istream &Require::deserialize(std::istream &buff) {
  buff.read(reinterpret_cast<char *>(&type_), sizeof(type_));
  decltype(hasId()) hasId_;
  buff.read(reinterpret_cast<char *>(&hasId_), sizeof(hasId_));
  if (hasId_) {
    id_ = std::make_unique<std::uint64_t>();
    buff.read(reinterpret_cast<char *>(id_.get()), sizeof(*id_));
  }
  decltype(hasMessage()) hasMessage_;
  buff.read(reinterpret_cast<char *>(&hasMessage_), sizeof(hasMessage_));
  if (hasMessage_) {
    decltype(message_->size()) size;
    buff.read(reinterpret_cast<char *>(&size), sizeof(size));
    message_ = std::make_unique<std::string>(size, '\0');
    if(size > 0)
      buff.read(&message_->at(0), size);
  }
  return buff;
}

std::ostream &Reply::serialize(std::ostream &buff) const {
  buff.write(reinterpret_cast<char const *>(&type_), sizeof(type_));
  auto hasTimestamp_ = hasTimestamp();
  buff.write(reinterpret_cast<char const *>(&hasTimestamp_), sizeof(hasTimestamp_));
  if (hasTimestamp_)
    buff.write(reinterpret_cast<char const *>(timestamp_.get()), sizeof(*timestamp_));

  auto hasName_ = hasName();
  buff.write(reinterpret_cast<char const *>(&hasName_), sizeof(hasName_));
  if (hasName_) {
    auto size = name_->size();
    buff.write(reinterpret_cast<char const *>(&size), sizeof(size));
    buff << *name_;
  }

  auto hasMessage_ = hasMessage();
  buff.write(reinterpret_cast<char const *>(&hasMessage_), sizeof(hasMessage_));
  if (hasMessage_) {
    auto size = message_->size();
    buff.write(reinterpret_cast<char const *>(&size), sizeof(size));
    buff << *message_;
  }

  buff << clients_;

  return buff;
}

std::istream &Reply::deserialize(std::istream &buff) {
  buff.read(reinterpret_cast<char *>(&type_), sizeof(type_));
  decltype(hasTimestamp()) hasTimestamp_;
  buff.read(reinterpret_cast<char *>(&hasTimestamp_), sizeof(hasTimestamp_));
  if (hasTimestamp_) {
    timestamp_ = std::make_unique<std::time_t>();
    buff.read(reinterpret_cast<char *>(timestamp_.get()), sizeof(*timestamp_));
  }
  decltype(hasName()) hasName_;
  buff.read(reinterpret_cast<char *>(&hasName_), sizeof(hasName_));
  if (hasName_) {
    decltype(name_->size()) size;
    buff.read(reinterpret_cast<char *>(&size), sizeof(size));
    name_ = std::make_unique<std::string>(size, '\0');
    if(size > 0)
      buff.read(&name_->at(0), size);
  }
  decltype(hasMessage()) hasMessage_;
  buff.read(reinterpret_cast<char *>(&hasMessage_), sizeof(hasMessage_));
  if (hasMessage_) {
    decltype(message_->size()) size;
    buff.read(reinterpret_cast<char *>(&size), sizeof(size));
    message_ = std::make_unique<std::string>(size, '\0');
    if(size > 0)
      buff.read(&message_->at(0), size);
  }
  buff >> clients_;
  return buff;
}

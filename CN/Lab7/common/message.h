//
// Created by lovesy on 18-4-22.
//

#ifndef COMMON_MESSAGE_H
#define COMMON_MESSAGE_H

#include <memory>
#include <utility>
#include <vector>
#include <queue>
#include <ostream>
#include <istream>
#include <memory>
#include <iostream>
#include <ctime>
#include "socket.h"

constexpr std::uint32_t MAGIC_NUMBER = 0x12041108;

enum Type : char {
  CLOSE = 0,
  TIME = 1,
  NAME = 2,
  CLIENT = 3,
  SEND = 4,
  RECEIVE = 5,
  HELLO = 6,
  ERROR = 7,
};

class Message {
 public:
  void sendTo(Socket &socket) const;
  void readFrom(Socket &socket);
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
class MessageArray : public std::enable_if_t<std::is_base_of_v<Message, T>, Message>, public std::vector<T> {
 protected:
  std::ostream &serialize(std::ostream &buff) const override;
  std::istream &deserialize(std::istream &buff) override;
};

template<class T>
std::istream &MessageArray<T>::deserialize(std::istream &buff) {
  size_t size_;
  buff.read(reinterpret_cast<char *>(&size_), sizeof(size_));
  std::vector<T>::resize(size_);
  for (auto &element : *this)
    element.deserialize(buff);
  return buff;
}
template<class T>
std::ostream &MessageArray<T>::serialize(std::ostream &buff) const {
  auto size_ = std::vector<T>::size();
  buff.write(reinterpret_cast<char *>(&size_), sizeof(size_));
  for (auto const &element : *this)
    element.serialize(buff);
  return buff;
}

class ClientMessage : public Message {
  template<typename>
  friend
  class MessageArray;
 public:
  ClientMessage() = default;
  ClientMessage(std::uint64_t const &id, std::string ip, std::string const port)
      : id_(id), ip_(std::move(ip)), service_(port) {};
  std::uint64_t id() const { return id_; }
  std::string service() const { return service_; }
  std::string ip() const { return ip_; }
 protected:
  std::ostream &serialize(std::ostream &buff) const override;
  std::istream &deserialize(std::istream &buff) override;
 private:
  std::uint64_t id_{0u};
  std::string ip_{};
  std::string service_{};
};

class Require : public Message {
 public:
  Require() = default;

  explicit Require(Type type) : type_(type) {}

  bool hasId() const {
    return id_ != nullptr;
  }

  bool hasMessage() const {
    return message_ != nullptr;
  }

  Type type() const { return type_; }
  std::uint64_t id() const { return *id_; }
  std::string message() const { return *message_; }

  void type(Type const &type) { type_ = type; }

  void id(std::uint64_t const &id) {
    id_ = std::make_unique<uint64_t>(id);
  }

  void message(std::string const &message) {
    message_ = std::make_unique<std::string>(message);
  }

 protected:
  std::ostream &serialize(std::ostream &buff) const override;
  std::istream &deserialize(std::istream &buff) override;
 private:
  Type type_;
  std::unique_ptr<std::uint64_t> id_{nullptr};
  std::unique_ptr<std::string> message_{nullptr};

};

class Reply : public Message {
 public:
  Reply() = default;
  explicit Reply(Type type) : type_(type) {}
  Type type() const { return type_; }
  bool hasTimestamp() const { return timestamp_ != nullptr; }
  bool hasName() const { return name_ != nullptr; }
  bool hasMessage() const { return message_ != nullptr; }
  void type(Type const &type) { type_ = type; }
  std::string message() const { return *message_; }
  void message(std::string const &message) {
    message_ = std::make_unique<std::string>(message);
  }
  std::time_t timestamp() const { return *timestamp_; }
  void timestamp(std::time_t const &timestamp) {
    timestamp_ = std::make_unique<std::time_t>(timestamp);
  }
  std::string name() const { return *name_; }
  void name(std::string const &name) {
    name_ = std::make_unique<std::string>(name);
  }

  void client(ClientMessage const &client) {
    clients_.push_back(client);
  }

  MessageArray<ClientMessage> &clients() {
    return clients_;
  }

  const MessageArray<ClientMessage> &clients() const {
    return clients_;
  }

 protected:
  std::ostream &serialize(std::ostream &buff) const override;
  std::istream &deserialize(std::istream &buff) override;
 private:
  Type type_;
  std::unique_ptr<std::time_t> timestamp_;
  std::unique_ptr<std::string> name_;
  MessageArray<ClientMessage> clients_;
  std::unique_ptr<std::string> message_;
};
#endif //COMMON_MESSAGE_H

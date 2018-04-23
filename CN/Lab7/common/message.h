//
// Created by lovesy on 18-4-22.
//

#ifndef COMMON_MESSAGE_H
#define COMMON_MESSAGE_H

#include <cstdint>
#include <string>
#include <ostream>
#include <istream>
#include <memory>

template<typename T, unsigned id_>
class Field {
  std::enable_if_t<!std::is_pointer_v<T>, T> value_;
 public:
  static const unsigned id = id_;
  using type = T;

  template<typename... U>
  Field(U &&... u) : value_(std::forward<U>(u)...) {} // NOLINT
  template<typename U>
  Field &operator=(U &u) { value_ = u; }

  operator const T &() const { return value_; } // NOLINT
  operator T &() { return value_; } // NOLINT
  template<typename U, unsigned id>
  friend std::ostream &operator<<(std::ostream &out, Field<U, id> const &field);

  template<unsigned id>
  friend std::ostream &operator<<(std::ostream &out, Field<std::string, id> const &field);

  template<typename U, unsigned id>
  friend std::istream &operator>>(std::istream &in, Field<U, id> &field);

  template<unsigned id>
  friend std::istream &operator>>(std::istream &in, Field<std::string, id> &field);

  decltype(auto) operator!() { return !value_; }
};

class Client {
  using id_type = Field<std::uint64_t, 1>;
  using ip_type = Field<std::string, 2>;
  using port_type = Field<std::uint16_t, 3>;
  using string = std::string;
  using uint16 = std::uint16_t;

  id_type id_;
  ip_type ip_;
  port_type port_;

 public:
  friend std::ostream &operator<<(std::ostream &out, Client const &client);

  friend std::istream &operator>>(std::istream &in, Client &client);

  inline Client(id_type::type const &id, ip_type::type ip, port_type::type const &port) : id_(id), ip_(std::move(ip)),
                                                                                          port_(port) {}

  inline Client() = default;

  inline id_type::type id() const {
    return id_;
  };

  inline ip_type::type ip() const {
    return ip_;
  }

  inline port_type::type port() const {
    return port_;
  }

  inline void id(id_type::type const &id) {
    id_type sii = 1;
    id_ = id;
  }

  inline void ip(port_type::type const &ip) {
    ip_ = ip;
  }

  inline void port(port_type::type const &port) {
    port_ = port;
  }

};

enum Type : unsigned char {
  TIME = 0u,
  Name,
  CLIENT,
  SEND,
  RECIEVE,
  ERROR,
};

class Require {
  using type_type = Field<Type, 1>;
  using id_type = Field<std::shared_ptr<uint64_t>, 2>;
  using message_type = Field<std::shared_ptr<std::string>, 3>;

  type_type type_;
  id_type id_{nullptr};
  message_type message_{nullptr};

 public:

  inline type_type::type type() {
    return type_;
  }

  inline id_type::type id() {
    return id_;
  }

  inline message_type::type message() {
    return message_;
  }

  inline void type(type_type::type type) {
    type_ = type;
  }

  inline void id(id_type::type id) {
    id_ = id;
  }

  inline void message(message_type::type::element_type message) {
    if (!message_)
      message_ = std::make_shared<>(message);
    message_ = message;
  }
};

#endif //COMMON_MESSAGE_H

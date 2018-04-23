#include <iostream>
#include <algorithm>
#include <sstream>
#include <iterator>
#include "message.h"

template<typename T>
inline std::enable_if_t<!std::is_pointer_v<T>, std::ostream &> operator<(std::ostream &out, T const &value) {
  std::copy_n(reinterpret_cast<const char *>(&value), sizeof(T), std::ostream_iterator<char>(out));
  return out;
}

template<typename T>
inline std::enable_if_t<!std::is_pointer_v<T>, std::istream &> operator>(std::istream &in, T &value) {
  std::copy_n(std::istream_iterator<char>(in), sizeof(T), reinterpret_cast<char *>(&value));
}

template<typename T, unsigned id_>
std::ostream &operator<<(std::ostream &out, Field<T, id_> const &field) {
  //out<id_;
  out < field.value_;
  return out;
};

template<unsigned id_>
std::ostream &operator<<(std::ostream &out, Field<std::string, id_> const &field) {
  //out<id_;
  auto size = field.value_.size();
  out < size;
  std::copy_n(field.value_.begin(), size, std::ostream_iterator<char>(out));
  return out;
}

template<typename T, unsigned id_>
std::istream &operator>>(std::istream &in, Field<T, id_> &field) {
  std::copy_n(std::istream_iterator<char>(in), sizeof(T), reinterpret_cast<char *>(&field.value_));
  return in;
}

template<unsigned id_>
std::istream &operator>>(std::istream &in, Field<std::string, id_> &field) {
  decltype(field.value_.size()) size;
  in > size;
  field.value_.resize(size);
  std::copy_n(std::istream_iterator<char>(in), size, field.value_.begin());
  return in;
}

std::ostream &operator<<(std::ostream &out, Client const &client) {
  auto id = Client::id_type::id;
  out < id;
  out < client.id_;
  id = Client::ip_type::id;
  out < id;
  out << client.ip_;
  id = Client::port_type::id;
  out < id;
  out << client.port_;
  return out;
}

std::istream &operator>>(std::istream &in, Client &client) {
  unsigned id;
  in > id;
  if (id != Client::id_type::id)
    throw std::exception();
  in >> client.id_;

  in > id;
  if (id != Client::ip_type::id)
    throw std::exception();
  in >> client.ip_;

  in > id;
  if (id != Client::port_type::id)
    throw std::exception();
  in >> client.port_;
  return in;
}

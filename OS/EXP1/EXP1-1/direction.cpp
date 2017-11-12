#include "direction.h"
#include <iostream>
using namespace std;

Direction::operator string() const {
  static std::string name[4] = {"East", "South", "West", "North"};
  return name[direction];
}

Direction::Direction(const char &c) {
  switch (c) {
  case 'e':
    direction = East;
    break;
  case 's':
    direction = South;
    break;
  case 'w':
    direction = West;
    break;
  case 'n':
    direction = North;
    break;
  }
}

Direction::Direction(const unsigned &d) {
  direction = static_cast<Directions>(d % 4);
}

Direction::operator Directions() const { return direction; }

const Direction Direction::right() const { return Direction(direction + 3); }

const Direction Direction::left() const { return Direction(direction + 1); }

const Direction Direction::opposite() const { return Direction(direction + 2); }

ostream &operator<<(ostream &out, const Direction &direction) {
  out << (string)(direction);
  return out;
}

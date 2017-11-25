#ifndef DIRECTION_H
#define DIRECTION_H
#include <ostream>
#include <string>

class Direction {
public:
  enum Directions : unsigned { East, South, West, North };

private:
  Directions direction;
  Direction(const unsigned &direction);

public:
  // a new direction for a char
  Direction(const char &c);
  //get other direction from itself
  const Direction left() const;
  const Direction right() const;
  const Direction opposite() const;
  operator Directions() const;
  // for outputing
  operator std::string() const;
  friend std::ostream &operator<<(std::ostream &out,
                                  const Direction &direction);
};
#endif

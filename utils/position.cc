#include "position.h"

Position::Position() : x{-1}, y{-1} {}

Position::Position(int x, int y) : x{x}, y{y} {}

Position::Position(const Position &other) : x{other.x}, y{other.y} {}

void Position::operator=(const Position &other) {
  x = other.x;
  y = other.y;
}

const std::pair<int, int> Position::getPosition() const {
  return std::make_pair(x, y);
}

void Position::setPosition(int x, int y) {
  this->x = x;
  this->y = y;
}

bool Position::operator==(const Position &other) const {
  return x == other.x && y == other.y;
}

bool Position::operator!=(const Position &other) const {
  return !(*this == other);
}

Position Position::operator+(const Position &other) const {
  return Position{x + other.x, y + other.y};
}

Position &Position::operator+=(const Position &other) {
  x += other.x;
  y += other.y;
  return *this;
}

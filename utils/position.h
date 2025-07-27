#ifndef POSITION_H
#define POSITION_H

#include <utility>

class Position {
    int x, y;
public:
    Position(int x, int y): x{x}, y{y} {}
    Position(const Position &other): x{other.x}, y{other.y} {}
    void operator=(const Position &other);
    ~Position();

    const std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    bool operator==(const Position &other) const;
    bool operator!=(const Position &other) const;
    Position operator+(const Position &other) const;
    Position& operator+=(const Position &other);
};

#endif // POSITION_H
#ifndef POSITION_H
#define POSITION_H

#include <utility>

class Position
{
    int x, y;

public:
    Position();
    Position(int x, int y);
    Position(const Position &other);
    void operator=(const Position &other);
    ~Position();

    const std::pair<int, int> getPosition() const;
    void setPosition(int x, int y);
    bool operator==(const Position &other) const;
    bool operator!=(const Position &other) const;
    Position operator+(const Position &other) const;
    Position &operator+=(const Position &other);
};

#endif // POSITION_H
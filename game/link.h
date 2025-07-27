#ifndef LINK_H
#define LINK_H

#include <vector>
#include "../utils/position.h"
#include "../utils/permission.h"

class Link {
    int type;
    int strength;
    std::vector<Position> moves;
    bool isDownloaded;
    Position position;
    Permission permission;

public:
    Link(int type, int strength, const std::vector<Position>& moves);
    ~Link();

    int getType() const;
    int getStrength() const;
    const std::vector<Position>& getMoves() const;
    Position getPosition() const;
    Permission getPermission() const;
    bool getIsDownloaded() const;
    
    void setMoves(const std::vector<Position>& moves);
    void setStrength(int strength);
    void setType(int type);
    void setPosition(const Position& position);
    void setPermission(const Permission& permission);
    void setIsDownloaded(bool isDownloaded);

    void notifyLinkDownloaded();
    void notifyLinkRevived();
    void notifyLinkStrengthChanged();
};

#endif // LINK_H
#ifndef LINK_H
#define LINK_H

#include <map>
#include "../utils/position.h"
#include "../utils/permission.h"
#include "occupant.h"

class Link final : public Occupant
{
    char name;
    int type;
    // 0: Data, 1: Virus
    int strength;
    std::map<std::string, Position> moves;
    bool isDownloaded;

public:
    Link(int type, int strength, Permission perm);
    ~Link();

    int getType() const;
    int getStrength() const;
    const std::map<std::string, Position> &getMoves() const;
    bool getIsDownloaded() const;

    bool canMove(const std::string &direction);
    void setMoves(const std::map<std::string, Position> &moves);
    void setStrength(int strength);
    void setType(int type);
    void setIsDownloaded(bool isDownloaded);

    void notifyLinkDownloaded();
    void notifyLinkRevived();
    void notifyLinkStrengthChanged();
};

#endif // LINK_H
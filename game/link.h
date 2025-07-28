#ifndef LINK_H
#define LINK_H

#include <map>
#include <string>
#include "../utils/position.h"
#include "../utils/permission.h"
#include "occupant.h"

class Link final : public Occupant
{
    int type;
    // 0: Data, 1: Virus
    int strength;
    std::map<std::string, Position> moves;
    bool isDownloaded;

public:
    char id;
    std::string name;
    Link(int type, int strength, const std::map<std::string, Position> &moves, const Permission &perm);
    ~Link();

    int getType() const;
    int getStrength() const;
    const std::map<std::string, Position> &getMoves() const;
    bool getIsDownloaded() const;

    void setMoves(const std::map<std::string, Position> &moves);
    void setStrength(int strength);
    void setType(int type);
    void setIsDownloaded(bool isDownloaded);

    void notifyLinkDownloaded();
    void notifyLinkRevived();
    void notifyLinkStrengthChanged();
};

#endif // LINK_H
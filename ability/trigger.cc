#include "trigger.h"
#include "../utils/payload.h"

using namespace std;

Trigger::Trigger(const Position& pos, const Permission& perm, function<void(const Payload&)> action) : 
    Occupant{pos, perm}, triggerAction{action} {}

Trigger::~Trigger() {}

void Trigger::trigger(const Payload& payload) {
    triggerAction(payload);
    notifyRuleUsed();
}

void Trigger::notifyRuleUsed() {
    // Implement this later
} 
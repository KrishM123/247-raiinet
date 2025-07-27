#include "trigger.h"

using namespace std;

Trigger::Trigger(function<void(Link&)> action) : Occupant{}, triggerAction{action} {}

Trigger::~Trigger() {}

void Trigger::trigger(Link& triggeredBy) {
    if (triggerAction) {
        triggerAction(triggeredBy);
        notifyRuleUsed();
    }
}

void Trigger::notifyRuleUsed() {
    // Implement this later
} 
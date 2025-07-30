#include "payload.h"

Payload::Payload() {}

Payload::Payload(const std::map<std::string, std::string> &payload)
    : payload{payload} {}

void Payload::add(const std::string &key, const std::string &value) {
  payload[key] = value;
}

void Payload::remove(const std::string &key) { payload.erase(key); }

std::string Payload::get(const std::string &key) const {
  auto it = payload.find(key);
  if (it != payload.end()) {
    return it->second;
  }
  return "";
}

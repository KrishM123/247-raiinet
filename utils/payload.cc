#include "payload.h"

Payload::Payload() {}

Payload::Payload(const std::map<std::string, std::string> &payload)
    : payload{payload} {}

Payload::~Payload() {}

std::map<std::string, std::string> Payload::getPayload() const {
  return payload;
}

void Payload::setPayload(const std::map<std::string, std::string> &payload) {
  this->payload = payload;
}

void Payload::add(const std::string &key, const std::string &value) {
  payload[key] = value;
}

void Payload::remove(const std::string &key) { payload.erase(key); }

void Payload::clear() { payload.clear(); }

bool Payload::contains(const std::string &key) const {
  return payload.find(key) != payload.end();
}

std::string Payload::get(const std::string &key) const {
  auto it = payload.find(key);
  if (it != payload.end()) {
    return it->second;
  }
  return "";
}
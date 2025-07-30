#ifndef PAYLOAD_H
#define PAYLOAD_H

#include <map>
#include <string>

class Payload {
  std::map<std::string, std::string> payload;

public:
  Payload();
  Payload(const std::map<std::string, std::string> &payload);

  void add(const std::string &key, const std::string &value);
  void remove(const std::string &key);
  std::string get(const std::string &key) const;
};

#endif // PAYLOAD_H

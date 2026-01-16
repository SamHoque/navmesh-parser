#pragma once
#include <unordered_map>
#include "buffer.h"

namespace navmesh {
/* unfinished */
class KV3Object
{
public:
  std::string                                  key;
  std::unordered_map<std::string, std::string> properties;

  explicit KV3Object(const std::string &k = "") : key(k) {}

  auto
  AddProperty(const std::string &name, const std::string &value) -> void
  {
    properties[name] = value;
  }

  auto
  get_property(const std::string &name,
               const std::string &default_value = "") const -> std::string
  {
    auto it = properties.find(name);
    if (it != properties.end())
      return it->second;

    return default_value;
  }

  auto
  has_property(const std::string &name) const -> bool
  {
    return properties.find(name) != properties.end();
  }
};

class KV3
{
public:
  static bool Skip(Buffer &buffer);
  static bool IsBinary(Buffer &buffer);

private:
  static bool ParseBinary(Buffer &buffer, KV3Object &result);
};
} // namespace navmesh

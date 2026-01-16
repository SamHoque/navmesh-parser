#pragma once
#include "buffer.h"

namespace navmesh {

class File;

class Area
{
public:
  struct Connect
  {
    Connect() = default;
    Connect(std::uint32_t area_id, std::uint32_t edge_id)
        : area_id(area_id), edge_id(edge_id)
    {}

    std::uint32_t area_id = 0;
    std::uint32_t edge_id = 0;
  };

  Area(Buffer &buffer, File &file);
  ~Area();

  Vector3_t Center();
  bool      IsWithin(const Vector3_t &position) const;

  auto
  id() const -> std::uint32_t
  {
    return id_;
  }

  auto
  connections() const -> const std::vector<std::vector<Connect>> &
  {
    return connections_;
  }
  auto
  corners() const -> const std::vector<Vector3_t> &
  {
    return corners_;
  }
  auto
  hull_index() const -> std::uint8_t
  {
    return hull_index_;
  }
  auto
  dynamic_attribute_flags() const -> std::int64_t
  {
    return dynamic_attribute_flags_;
  }
  auto
  ladders_above() const -> const std::vector<std::uint32_t> &
  {
    return ladders_above_;
  }
  auto
  ladders_below() const -> const std::vector<std::uint32_t> &
  {
    return ladders_below_;
  }

private:
  void Load(Buffer &buffer, File &file);
  void Unload();

  std::uint32_t id_ = 0;
  std::int64_t  dynamic_attribute_flags_ = 0;
  std::uint8_t  hull_index_ = 0;

  std::vector<Vector3_t>            corners_;
  std::vector<std::vector<Connect>> connections_;
  std::vector<std::uint32_t>        ladders_above_;
  std::vector<std::uint32_t>        ladders_below_;
};
} // namespace navmesh

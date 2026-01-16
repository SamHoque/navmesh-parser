#include <algorithm>
#include "area.h"
#include "file.h"

namespace navmesh {
Area::Area(Buffer &buffer, File &file) { Load(buffer, file); }

Area::~Area() { Unload(); }

auto
Area::Center() -> Vector3_t
{
  if (corners_.empty())
    return {};

  auto center = Vector3_t{};
  for (const auto &corner : corners_)
    center = center + corner;

  center = center / static_cast<float>(corners_.size());
  return center;
}

auto
Area::IsWithin(const Vector3_t &position) const -> bool
{
  if (corners_.size() < 3)
    return false;

  auto min = Vector2_t{std::numeric_limits<float>::max(),
                       -std::numeric_limits<float>::max()};
  auto max = Vector2_t{std::numeric_limits<float>::max(),
                       -std::numeric_limits<float>::max()};

  for (const auto &corner : corners_) {
    if (corner.at(0) < min.at(0))
      min.at(0) = corner.at(0);

    if (corner.at(0) > min.at(1))
      min.at(1) = corner.at(0);

    if (corner.at(1) < max.at(0))
      max.at(0) = corner.at(1);

    if (corner.at(1) > max.at(1))
      max.at(1) = corner.at(1);
  }

  if (position.at(0) < min.at(0) || position.at(0) > min.at(1) ||
      position.at(1) < max.at(0) || position.at(1) > max.at(1))
    return false;

  /* TODO: Implement proper point-in-polygon test*/
  return true;
}

auto
Area::Load(Buffer &buffer, File &file) -> void
{
  id_ = buffer.Read<std::uint32_t>();
  dynamic_attribute_flags_ = buffer.Read<std::int64_t>();
  hull_index_ = buffer.Read<std::uint8_t>();

  if (file.version() >= 31) {
    auto        polygon_index = buffer.Read<std::uint32_t>();
    const auto &polygons = file.polygons();

    if (polygon_index < polygons.size())
      corners_ = polygons[polygon_index];
    else
      throw std::exception("Area::Load: polygon index out of range");
  } else {
    auto corner_count = buffer.Read<std::uint32_t>();
    corners_.resize(corner_count);

    for (std::uint32_t i = 0; i < corner_count; ++i) {
      corners_[i].at(0) = buffer.Read<float>();
      corners_[i].at(1) = buffer.Read<float>();
      corners_[i].at(2) = buffer.Read<float>();
    }
  }

  buffer.Read<float>();

  connections_.resize(corners_.size());
  for (std::size_t i = 0; i < corners_.size(); ++i) {
    auto connection_count = buffer.Read<std::uint32_t>();
    connections_[i].resize(connection_count);

    for (std::uint32_t j = 0; j < connection_count; ++j) {
      connections_[i][j].area_id = buffer.Read<std::uint32_t>();
      connections_[i][j].edge_id = buffer.Read<std::uint32_t>();
    }
  }

  [[maybe_unused]] auto unk2 = buffer.Read<std::uint8_t>();
  [[maybe_unused]] auto unk3 = buffer.Read<std::uint32_t>();

  auto ladder_above_count = buffer.Read<std::uint32_t>();
  ladders_above_.resize(ladder_above_count);
  for (std::uint32_t i = 0; i < ladder_above_count; ++i)
    ladders_above_[i] = buffer.Read<std::uint32_t>();

  auto ladder_below_count = buffer.Read<std::uint32_t>();
  ladders_below_.resize(ladder_below_count);
  for (std::uint32_t i = 0; i < ladder_below_count; ++i)
    ladders_below_[i] = buffer.Read<std::uint32_t>();
}

auto
Area::Unload() -> void
{
  id_ = 0;
  dynamic_attribute_flags_ = 0;
  hull_index_ = 0;

  corners_.clear();
  connections_.clear();
  ladders_above_.clear();
  ladders_below_.clear();
}
} // namespace navmesh

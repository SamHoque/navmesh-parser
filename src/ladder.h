#pragma once
#include "buffer.h"

namespace navmesh {
class File;
class Ladder
{
public:
  Ladder() = default;
  Ladder(Buffer &buffer, File &file);
  ~Ladder();

  enum class Direction : std::uint32_t
  {
    kNorth = 0,
    kEast = 1,
    kSouth = 2,
    kWest = 3,
  };

  auto
  id() const -> std::uint32_t
  {
    return id_;
  }
  auto
  width() const -> float
  {
    return width_;
  }
  auto
  length() const -> float
  {
    return length_;
  }
  auto
  top() const -> const Vector3_t &
  {
    return top_;
  }
  auto
  bottom() const -> const Vector3_t &
  {
    return bottom_;
  }
  auto
  direction() const -> Direction
  {
    return direction_;
  }
  auto
  top_forward_area_id() const -> std::uint32_t
  {
    return top_forward_area_id_;
  }
  auto
  top_left_area_id() const -> std::uint32_t
  {
    return top_left_area_id_;
  }
  auto
  top_right_area_id() const -> std::uint32_t
  {
    return top_right_area_id_;
  }
  auto
  top_behind_area_id() const -> std::uint32_t
  {
    return top_behind_area_id_;
  }
  auto
  bottom_area_id() const -> std::uint32_t
  {
    return bottom_area_id_;
  }
  auto
  bottom_left_area_id() const -> std::uint32_t
  {
    return bottom_left_area_id_;
  }
  auto
  bottom_right_area_id() const -> std::uint32_t
  {
    return bottom_right_area_id_;
  }

private:
  void Load(Buffer &buffer, File &file);
  void Unload();

  std::uint32_t id_ = 0;
  float         width_ = 0.0f;
  float         length_ = 0.0f;
  Vector3_t     top_ = {};
  Vector3_t     bottom_ = {};
  Direction     direction_ = Direction::kNorth;

  std::uint32_t top_forward_area_id_ = 0;
  std::uint32_t top_left_area_id_ = 0;
  std::uint32_t top_right_area_id_ = 0;
  std::uint32_t top_behind_area_id_ = 0;
  std::uint32_t bottom_area_id_ = 0;
  std::uint32_t bottom_left_area_id_ = 0;
  std::uint32_t bottom_right_area_id_ = 0;
};
} // namespace navmesh

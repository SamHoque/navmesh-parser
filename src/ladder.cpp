#include "ladder.h"
#include "file.h"

namespace navmesh {
Ladder::Ladder(Buffer &buffer, File &file) { Load(buffer, file); }

Ladder::~Ladder() { Unload(); }

auto
Ladder::Load(Buffer &buffer, File &file) -> void
{
  id_ = buffer.Read<std::uint32_t>();
  width_ = buffer.Read<float>();

  top_.at(0) = buffer.Read<float>();
  top_.at(1) = buffer.Read<float>();
  top_.at(2) = buffer.Read<float>();

  bottom_.at(0) = buffer.Read<float>();
  bottom_.at(1) = buffer.Read<float>();
  bottom_.at(2) = buffer.Read<float>();

  length_ = buffer.Read<float>();
  direction_ = static_cast<Direction>(buffer.Read<std::uint32_t>());

  top_forward_area_id_ = buffer.Read<std::uint32_t>();
  top_left_area_id_ = buffer.Read<std::uint32_t>();
  top_right_area_id_ = buffer.Read<std::uint32_t>();
  top_behind_area_id_ = buffer.Read<std::uint32_t>();
  bottom_area_id_ = buffer.Read<std::uint32_t>();

  if (file.version() >= 35) {
    bottom_left_area_id_ = buffer.Read<std::uint32_t>();
    bottom_right_area_id_ = buffer.Read<std::uint32_t>();
  }
}

auto
Ladder::Unload() -> void
{
  id_ = 0;
  width_ = 0.0f;
  length_ = 0.0f;
  top_ = {};
  bottom_ = {};
  direction_ = Direction::kNorth;

  top_forward_area_id_ = 0;
  top_left_area_id_ = 0;
  top_right_area_id_ = 0;
  top_behind_area_id_ = 0;
  bottom_area_id_ = 0;
  bottom_left_area_id_ = 0;
  bottom_right_area_id_ = 0;
}
} // namespace navmesh

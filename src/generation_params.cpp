#include "file.h"

namespace navmesh {
auto
GenerationHullParams::Load(Buffer                 &buffer,
                           const GenerationParams &parent) -> void
{
  if (parent.nav_gen_version >= 9)
    enabled = buffer.Read<std::uint8_t>() > 0;

  radius = buffer.Read<float>();
  height = buffer.Read<float>();

  if (parent.nav_gen_version >= 9) {
    short_height_enabled = buffer.Read<std::uint8_t>() > 0;
    short_height = buffer.Read<float>();
  }

  if (parent.nav_gen_version >= 13)
    buffer.Skip(5);

  max_climb = buffer.Read<float>();
  max_slope = buffer.Read<std::int32_t>();
  max_jump_down_dist = buffer.Read<float>();
  max_jump_horiz_dist_base = buffer.Read<float>();
  max_jump_up_dist = buffer.Read<float>();

  if (parent.nav_gen_version >= 11)
    border_erosion = buffer.Read<std::int32_t>();
}

auto
GenerationParams::Load(Buffer &buffer, File &file) -> void
{
  nav_gen_version = buffer.Read<std::int32_t>();
  use_project_defaults = buffer.Read<std::uint32_t>() != 0;

  tile_size = buffer.Read<float>();
  cell_size = buffer.Read<float>();
  cell_height = buffer.Read<float>();

  min_region_size = buffer.Read<std::int32_t>();
  merged_region_size = buffer.Read<std::int32_t>();

  mesh_sample_distance = buffer.Read<float>();
  max_sample_error = buffer.Read<float>();

  max_edge_length = buffer.Read<std::int32_t>();
  max_edge_error = buffer.Read<float>();
  verts_per_poly = buffer.Read<std::int32_t>();

  if (nav_gen_version >= 7)
    small_area_on_edge_removal = buffer.Read<float>();

  if (nav_gen_version >= 12) {
    hull_preset_name = file.ReadNullTerminatedString();
    hull_definitions_file = file.ReadNullTerminatedString();
  }

  hull_count = buffer.Read<std::int32_t>();
  hull_params.resize(hull_count);

  for (auto i = 0; i < hull_count; ++i)
    hull_params[i].Load(buffer, *this);

  if (nav_gen_version <= 11) {
    GenerationHullParams temp;
    for (auto i = hull_count; i < 3; ++i)
      temp.Load(buffer, *this);
  }

  [[maybe_unused]] std::uint8_t unk_byte = 0;
  if (nav_gen_version >= 12)
    unk_byte = buffer.Read<std::uint8_t>();
}
} // namespace navmesh

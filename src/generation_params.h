#pragma once
#include <cstdint>
#include <vector>
#include <string>

namespace navmesh {
class Buffer;
class File;
class GenerationParams;
struct GenerationHullParams
{
  bool         enabled = true;
  float        radius = 0.0f;
  float        height = 0.0f;
  bool         short_height_enabled = false;
  float        short_height = 0.0f;
  float        max_climb = 0.0f;
  std::int32_t max_slope = 0;
  float        max_jump_down_dist = 0.0f;
  float        max_jump_horiz_dist_base = 0.0f;
  float        max_jump_up_dist = 0.0f;
  std::int32_t border_erosion = 0;

  void Load(Buffer &buffer, const GenerationParams &parent);
};

class GenerationParams
{
public:
  std::int32_t                      nav_gen_version = 0;
  bool                              use_project_defaults = false;
  float                             tile_size = 0.0f;
  float                             cell_size = 0.0f;
  float                             cell_height = 0.0f;
  std::int32_t                      min_region_size = 0;
  std::int32_t                      merged_region_size = 0;
  float                             mesh_sample_distance = 0.0f;
  float                             max_sample_error = 0.0f;
  std::int32_t                      max_edge_length = 0;
  float                             max_edge_error = 0.0f;
  std::int32_t                      verts_per_poly = 0;
  float                             small_area_on_edge_removal = 0.0f;
  std::string                       hull_preset_name;
  std::string                       hull_definitions_file;
  std::int32_t                      hull_count = 0;
  std::vector<GenerationHullParams> hull_params;

  void Load(Buffer &buffer, File &file);
};
} // namespace navmesh

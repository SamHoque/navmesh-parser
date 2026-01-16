#include <iostream>
#include "file.h"

namespace {
auto
HasNavExtension(std::string_view file_path) -> bool
{
  constexpr std::string_view kNavExtension = ".nav";
  if (file_path.length() < kNavExtension.length())
    return false;
  return file_path.substr(file_path.length() - kNavExtension.length()) ==
         kNavExtension;
}
} // namespace

auto
main(int argc, char *argv[]) -> int
{
  if (argc < 2) {
    std::cerr << "Usage: " << (argv[0] ? argv[0] : "wtf?") << " <navfile>\n";
    std::cerr << "Note: Place the .nav file in the executable directory or "
                 "provide full path.\n";
    return 1;
  }

  const auto nav_path = argv[1];
  if (!HasNavExtension(nav_path))
    std::cerr << "Warning: File '" << nav_path
              << "' does not have .nav extension.\n";

  auto navmesh = std::make_unique<navmesh::File>(nav_path);
  std::cout << "Area count: " << navmesh->area_count() << '\n';
  std::cout << "File version: " << navmesh->version() << '\n';
  std::cout << "Is analyzed: " << std::boolalpha << navmesh->is_analyzed()
            << '\n';

  Vector3_t position{100.0f, 200.0f, 50.0f};
  try {
    auto &area = navmesh->area_by_position(position);
    std::cout << "Found area ID: " << area.id() << '\n';

    const auto &connections = area.connections();
    std::cout << "Connection count at first edge: " << connections[0].size()
              << '\n';
  } catch (...) {
    std::cerr << "No area found at position: " << position.at(0) << ", "
              << position.at(1) << ", " << position.at(2) << '\n';
  }

  auto ladders = navmesh->ladders();
  std::cout << "Ladder count: " << ladders.size() << '\n';

  if (!ladders.empty()) {
    const auto &first_ladder = ladders[0];
    std::cout << "First ladder ID: " << first_ladder.id() << '\n';
    std::cout << "First ladder width: " << first_ladder.width() << '\n';
    std::cout << "First ladder length: " << first_ladder.length() << '\n';
  }

  return 0;
}

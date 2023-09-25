#include <algorithm>
#include <array>
#include <istream>
#include <limits>
#include <optional>
#include <queue>
#include <set>
#include <string>

namespace {
struct Landscape {
  std::vector<std::size_t> elevation;
  std::size_t start{0};
  std::size_t end{0};
  std::size_t width{0};

  void invert_elevations() {
    std::transform(elevation.begin(), elevation.end(), elevation.begin(),
                   [](auto x) { return 'z' - 'a' - x; });
  }
};

Landscape read_landscape(std::istream *input) {
  Landscape scape;
  for (std::string line; std::getline(*input, line);) {
    scape.width = line.size();
    for (auto ch : line) {
      if (ch == 'S') {
        scape.start = scape.elevation.size();
        scape.elevation.emplace_back(0);
      } else if (ch == 'E') {
        scape.end = scape.elevation.size();
        scape.elevation.emplace_back(25);
      } else {
        scape.elevation.emplace_back(ch - 'a');
      }
    }
  }
  return scape;
}

std::vector<std::vector<std::size_t>>
make_adjacency_list(const std::vector<std::size_t> &elevations,
                    const std::size_t width) {
  std::vector<std::vector<std::size_t>> adjacency_list(elevations.size());
  for (auto node_num = 0U; node_num < elevations.size(); ++node_num) {
    adjacency_list[node_num].reserve(4);
    std::size_t i = node_num / width;
    std::size_t j = node_num % width;
    constexpr std::array<std::array<int, 2>, 4> offsets = {
        {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
    for (const auto [i_offset, j_offset] : offsets) {
      int neighbour_i = i + i_offset;
      int neighbour_j = j + j_offset;
      auto neighbour_node_num = width * neighbour_i + neighbour_j;
      if (neighbour_i < 0 || neighbour_j < 0 ||
          static_cast<std::size_t>(neighbour_i) >= elevations.size() / width ||
          static_cast<std::size_t>(neighbour_j) >= width) {
        continue;
      }
      if (elevations[neighbour_node_num] <= elevations[node_num] + 1) {
        adjacency_list[node_num].emplace_back(neighbour_node_num);
      }
    }
  }
  return adjacency_list;
}

std::vector<std::size_t>
breadth_first_search(const std::vector<std::vector<std::size_t>> &adjacency,
                     const std::size_t start,
                     const std::optional<std::size_t> &target = std::nullopt) {
  std::queue<std::size_t> queue;
  queue.push(start);
  std::vector<bool> explored(adjacency.size(), false);
  explored[start] = true;
  std::vector<std::size_t> distance_to(adjacency.size(), 0);

  while (!queue.empty()) {
    auto node = queue.front();
    queue.pop();
    if (target && node == target) {
      return {distance_to[node]};
    }
    for (auto neighbour : adjacency[node]) {
      if (!explored[neighbour]) {
        queue.push(neighbour);
        explored[neighbour] = true;
        distance_to[neighbour] = distance_to[node] + 1;
      }
    }
  }
  return distance_to;
}
} // namespace

std::size_t day12_1(std::istream *input_file) {
  auto landscape = read_landscape(input_file);
  auto adj_list = make_adjacency_list(landscape.elevation, landscape.width);
  auto distance = breadth_first_search(adj_list, landscape.start, landscape.end);
  return distance.at(0);
}

std::size_t day12_2(std::istream *input_file) {
  auto landscape = read_landscape(input_file);
  landscape.invert_elevations();
  auto adj_list = make_adjacency_list(landscape.elevation, landscape.width);
  auto distances = breadth_first_search(adj_list, landscape.end);

  auto min_dist = std::numeric_limits<std::size_t>::max();
  ;
  for (auto i = 0U; i < landscape.elevation.size(); ++i) {
    if (landscape.elevation[i] == 25 && distances[i] != 0) {
      min_dist = std::min(min_dist, distances[i]);
    }
  }
  return min_dist;
}

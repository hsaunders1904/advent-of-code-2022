#include "aoc22/aoc.h"

#include <array>
#include <queue>
#include <set>

namespace {
struct Landscape {
  std::vector<std::size_t> elevation;
  std::size_t start;
  std::size_t end;
  std::size_t width;

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

std::vector<std::set<std::size_t>>
make_adjacency_list(const std::vector<std::size_t> &elevations, std::size_t width) {
  std::vector<std::set<std::size_t>> adjacency_list(elevations.size());
  for (auto node_num = 0U; node_num < elevations.size(); ++node_num) {
    std::size_t i = node_num / width;
    std::size_t j = node_num % width;
    constexpr std::array<std::array<int, 2>, 4> offsets = {
        {{1, 0}, {-1, 0}, {0, 1}, {0, -1}}};
    for (const auto [i_offset, j_offset] : offsets) {
      int neighbour_i = i + i_offset;
      int neighbour_j = j + j_offset;
      auto neighbour_node_num = width * neighbour_i + neighbour_j;
      if (neighbour_i < 0 || neighbour_j < 0 ||
          neighbour_node_num >= elevations.size()) {
        continue;
      }
      if (elevations[neighbour_node_num] <= elevations[node_num] + 1) {
        adjacency_list[node_num].insert(neighbour_node_num);
      }
    }
  }
  return adjacency_list;
}

std::vector<std::size_t>
breadth_first_search(const std::vector<std::set<std::size_t>> &adjacency,
                     std::size_t start,
                     std::optional<std::size_t> target = std::nullopt) {
  std::queue<std::size_t> queue;
  queue.push(start);
  std::vector<bool> explored(adjacency.size(), false);
  explored[start] = true;
  std::vector<std::size_t> distances(adjacency.size(), 0);

  while (!queue.empty()) {
    auto u = queue.front();
    queue.pop();
    if (target && u == target) {
      return {distances[u]};
    }
    for (auto neighbour : adjacency[u]) {
      if (!explored[neighbour]) {
        queue.push(neighbour);
        explored[neighbour] = true;
        distances[neighbour] = distances[u] + 1;
      }
    }
  }
  return distances;
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

  auto min_dist = SIZE_T_MAX;
  for (auto i = 0U; i < landscape.elevation.size(); ++i) {
    if (landscape.elevation[i] == 25 && distances[i] != 0) {
      min_dist = std::min(min_dist, distances[i]);
    }
  }
  return min_dist;
}

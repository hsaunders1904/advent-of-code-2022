#include "aoc22/aoc.h"

#include <array>
#include <list>
#include <set>

namespace {
std::tuple<std::vector<std::vector<std::size_t>>, std::array<std::size_t, 2>,
           std::array<std::size_t, 2>>
read_elevations(std::istream *input) {
  std::vector<std::vector<std::size_t>> elevations;
  std::array<std::size_t, 2> start{0};
  std::array<std::size_t, 2> end{0};

  std::size_t row{0};
  for (std::string line; std::getline(*input, line); ++row) {
    elevations.emplace_back(line.size());
    for (auto col = 0U; col < line.size(); ++col) {
      if (line[col] == 'S') {
        start = {row, col};
        elevations[row][col] = 0;
      } else if (line[col] == 'E') {
        end = {row, col};
        elevations[row][col] = 25;
      } else {
        elevations[row][col] = line[col] - 'a';
      }
    }
  }
  return {elevations, start, end};
}

std::vector<std::set<std::size_t>>
make_adjacency_list(const std::vector<std::vector<std::size_t>> &elevations) {
  const std::size_t height = elevations.size();
  const std::size_t width = elevations[0].size();
  std::vector<std::set<std::size_t>> adjacency_list(width * height);
  for (auto i = 0U; i < elevations.size(); ++i) {
    for (auto j = 0U; j < elevations[i].size(); ++j) {
      auto elevation = elevations[i][j];
      auto node_num = elevations[i].size() * i + j;
      // Check left
      if (j != 0) {
        auto l_elevation = elevations[i][j - 1];
        auto l_node_num = width * i + (j - 1);
        if (l_elevation <= elevation + 1) {
          adjacency_list[node_num].insert(l_node_num);
        }
      }
      // Check up
      if (i != 0) {
        auto u_elevation = elevations[i - 1][j];
        auto u_node_num = width * (i - 1) + j;
        if (u_elevation <= elevation + 1) {
          adjacency_list[node_num].insert(u_node_num);
        }
      }
      // Check right
      if (j != width - 1) {
        auto r_elevation = elevations[i][j + 1];
        auto r_node_num = width * i + j + 1;
        if (r_elevation <= elevation + 1) {
          adjacency_list[node_num].insert(r_node_num);
        }
      }
      // Check down
      if (i != height - 1) {
        auto d_elevation = elevations[i + 1][j];
        auto d_node_num = width * (i + 1) + j;
        if (d_elevation <= elevation + 1) {
          adjacency_list[node_num].insert(d_node_num);
        }
      }
    }
  }
  return adjacency_list;
}

std::vector<std::size_t>
dijkstra(const std::vector<std::set<std::size_t>> &adjacency,
         std::size_t start) {
  std::vector<std::size_t> dist(adjacency.size(),
                                std::numeric_limits<std::size_t>().max() - 1);
  dist[start] = 0;
  std::list<std::size_t> unvisited;
  for (auto v = 0U; v < adjacency.size(); ++v) {
    unvisited.push_back(v);
  }

  while (!unvisited.empty()) {
    auto min_path_it =
        std::min_element(unvisited.begin(), unvisited.end(),
                         [&dist](auto m, auto v) { return dist[m] < dist[v]; });
    auto min_path = *min_path_it;
    unvisited.erase(min_path_it);

    for (auto neighbour : adjacency[min_path]) {
      auto alt = dist[min_path] + 1;
      if (alt < dist[neighbour]) {
        dist[neighbour] = alt;
      }
    }
  }
  return dist;
}

void invert_elevations(std::vector<std::vector<std::size_t>> *elevations) {
  for (auto &row : *elevations) {
    for (auto &el : row) {
      el = 'z' - 'a' - el;
    }
  }
}
} // namespace

int day12_1(std::istream *input_file) {
  const auto [elevations, start, end] = read_elevations(input_file);
  auto width = elevations[0].size();
  const auto adjacency_list = make_adjacency_list(elevations);
  const auto distances = dijkstra(adjacency_list, start[0] * width + start[1]);
  return distances[end[0] * width + end[1]];
}

int day12_2(std::istream *input_file) {
  auto [elevations, start, end] = read_elevations(input_file);
  invert_elevations(&elevations);
  auto width = elevations[0].size();
  const auto adjacency_list = make_adjacency_list(elevations);
  const auto distances = dijkstra(adjacency_list, end[0] * width + end[1]);

  auto min_dist = std::numeric_limits<std::size_t>().max();
  for (auto row_idx = 0U; row_idx < elevations.size(); ++row_idx) {
    for (auto col_idx = 0U; col_idx < width; ++col_idx) {
      auto dist = distances[row_idx * width + col_idx];
      if (elevations[row_idx][col_idx] == 25 && dist < min_dist) {
        min_dist = dist;
      }
    }
  }
  return min_dist;
}

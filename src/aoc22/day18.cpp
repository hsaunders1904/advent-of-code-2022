#include "aoc22/utils.h"

#include <istream>
#include <numeric>
#include <queue>
#include <unordered_set>

namespace {
using Point = std::array<int, 3>;

bool are_neighbours(const Point &a, const Point &b) {
  int squared_distance = std::pow((a[0] - b[0]), 2) + std::pow((a[1] - b[1]), 2) +
                         std::pow((a[2] - b[2]), 2);
  return squared_distance == 1;
}

std::vector<Point> read_cubes_to_vector(std::istream *input_file) {
  std::vector<Point> cubes;
  for (std::string line; std::getline(*input_file, line);) {
    auto parts = split(line, ',');
    Point p = {{std::stoi(parts[0]), std::stoi(parts[1]), std::stoi(parts[2])}};
    cubes.emplace_back(p);
  }
  return cubes;
}

std::unordered_set<Point, SequenceHasher<Point>>
read_cubes_to_set(std::istream *input_file) {
  std::unordered_set<Point, SequenceHasher<Point>> cubes;
  for (std::string line; std::getline(*input_file, line);) {
    auto parts = split(line, ',');
    cubes.insert({std::stoi(parts[0]), std::stoi(parts[1]), std::stoi(parts[2])});
  }
  return cubes;
}

template <typename T>
std::array<std::array<int, 2>, 3> find_cube_limits(const T &cubes) {
  std::array<std::array<int, 2>, 3> limits = {{{std::numeric_limits<int>::max(), 0}}};
  for (const auto &cube : cubes) {
    for (auto i = 0; i < 3; ++i) {
      limits[i][0] = std::min(limits[i][0], cube[i]);
      limits[i][1] = std::max(limits[i][1], cube[i]);
    }
  }
  return limits;
}
} // namespace

int day18_1(std::istream *input_file) {
  auto cubes = read_cubes_to_vector(input_file);

  std::vector<std::size_t> n_neighbours(cubes.size());
  for (auto i = 0U; i < cubes.size(); ++i) {
    for (const auto &candidate_neighbour : cubes) {
      if (are_neighbours(cubes[i], candidate_neighbour)) {
        n_neighbours[i]++;
      }
    }
  }
  int max_n_free_faces = 6 * n_neighbours.size();
  return std::accumulate(n_neighbours.begin(), n_neighbours.end(), max_n_free_faces,
                         [](auto acc, auto x) { return acc - x; });
}

int day18_2(std::istream *input_file) {
  auto cubes = read_cubes_to_set(input_file);
  auto limits = find_cube_limits(cubes);

  int n_exposed_faces{0};
  std::queue<Point> positions;
  positions.push({limits[0][0] - 1, limits[1][0] - 1, limits[2][0] - 1});
  std::unordered_set<Point, SequenceHasher<Point>> visited;
  while (positions.size() > 0) {
    auto position = positions.front();
    positions.pop();

    for (auto dimension = 0U; dimension < 3; ++dimension) {
      for (auto direction : {-1, 1}) {
        auto candidate_position = position;
        candidate_position[dimension] += direction;
        // If we've already visited this coordinate, skip it
        if (visited.contains(candidate_position)) {
          continue;
        }
        // If the new coordinate is out-of-bounds, skip it
        if (candidate_position[dimension] < limits[dimension][0] - 1 ||
            candidate_position[dimension] > limits[dimension][1] + 1) {
          continue;
        }
        // If same coordinate as a droplet, increment the face counter and skip
        if (cubes.contains(candidate_position)) {
          n_exposed_faces += 1;
          continue;
        }
        positions.push(candidate_position);
        visited.insert(candidate_position);
      }
    }
  }
  return n_exposed_faces;
}

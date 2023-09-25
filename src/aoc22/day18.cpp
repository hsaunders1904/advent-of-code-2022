#include "aoc22/utils.h"

#include <istream>
#include <numeric>
#include <queue>
#include <unordered_set>

namespace {
struct Cube {
  Cube(const std::string &s) {
    auto parts = split(s, ',');
    x = std::stoi(parts.at(0));
    y = std::stoi(parts.at(1));
    z = std::stoi(parts.at(2));
  }

  int x;
  int y;
  int z;
};

template <std::size_t N> struct ArrayHasher {
  std::size_t operator()(const std::array<int, N> &a) const {
    std::size_t h = 0;
    for (auto e : a) {
      h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
  }
};

std::vector<Cube> read_cubes(std::istream *input_file) {
  std::vector<Cube> cubes;
  for (std::string line; std::getline(*input_file, line);) {
    cubes.emplace_back(line);
  }
  return cubes;
}

bool are_neighbours(const Cube &a, const Cube &b) {
  int squared_distance =
      std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2) + std::pow((a.z - b.z), 2);
  return squared_distance == 1;
}
} // namespace

int day18_1(std::istream *input_file) {
  auto cubes = read_cubes(input_file);

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
  std::unordered_set<std::array<int, 3>, ArrayHasher<3>> cubes;
  for (std::string line; std::getline(*input_file, line);) {
    auto parts = split(line, ',');
    cubes.insert({std::stoi(parts[0]), std::stoi(parts[1]), std::stoi(parts[2])});
  }

  // Get the limits of the droplet positions
  std::array<std::array<int, 2>, 3> limits = {{{std::numeric_limits<int>::max(), 0}}};
  for (const auto &cube : cubes) {
    for (auto i = 0; i < 3; ++i) {
      limits[i][0] = std::min(limits[i][0], cube[i]);
      limits[i][1] = std::max(limits[i][1], cube[i]);
    }
  }

  int n_exposed_faces{0};
  std::queue<std::array<int, 3>> positions;
  positions.push({limits[0][0] - 1, limits[1][0] - 1, limits[2][0] - 1});
  std::unordered_set<std::array<int, 3>, ArrayHasher<3>> visited;
  while (positions.size() > 0) {
    auto position = positions.front();
    positions.pop();

    for (auto i = 0U; i < 3; ++i) {
      for (auto direction : {-1, 1}) {
        auto candidate_position = position;
        candidate_position[i] += direction;
        // If we've already visited this coordinate, skip it
        if (visited.contains(candidate_position)) {
          continue;
        }
        // If the new coordinate is out-of-bounds, skip it
        if (candidate_position[i] < limits[i][0] - 1 ||
            candidate_position[i] > limits[i][1] + 1) {
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

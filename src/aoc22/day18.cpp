#include "aoc22/utils.h"

#include <istream>
#include <numeric>

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

bool are_neighbours(const Cube &a, const Cube &b) {
  int squared_dist =
      std::pow((a.x - b.x), 2) + std::pow((a.y - b.y), 2) + std::pow((a.z - b.z), 2);
  return squared_dist == 1;
}
} // namespace

int day18_1(std::istream *input_file) {
  std::vector<Cube> cubes;
  for (std::string line; std::getline(*input_file, line);) {
    cubes.emplace_back(line);
  }

  std::vector<std::size_t> n_neighbours(cubes.size());
  for (auto i = 0U; i < cubes.size(); ++i) {
    for (const auto &candidate_neighbour : cubes) {
      if (are_neighbours(cubes.at(i), candidate_neighbour)) {
        n_neighbours[i]++;
      }
    }
  }
  int max_n_free_faces = 6 * n_neighbours.size();
  return std::accumulate(n_neighbours.begin(), n_neighbours.end(), max_n_free_faces,
                         [](auto acc, auto x) { return acc - x; });
}

int day18_2(std::istream *input_file) { return 0; }

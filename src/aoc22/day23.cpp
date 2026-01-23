#include "aoc22/utils.h"

#include <algorithm>
#include <array>
#include <bitset>
#include <cstddef>
#include <istream>
#include <limits>
#include <stdexcept>
#include <unordered_map>
#include <unordered_set>

constexpr std::size_t NORTH{0};
constexpr std::size_t EAST{2};
constexpr std::size_t SOUTH{4};
constexpr std::size_t WEST{6};

struct Point {
  constexpr Point() = default;
  constexpr Point(int x, int y) : x(x), y(y) {}
  bool operator==(const Point &other) const = default;

  int x{0};
  int y{0};
};
struct PointHasher {
  PointHasher() = default;
  std::size_t operator()(const Point &p) const {
    return SequenceHasher<std::array<int, 2>>()({p.x, p.y});
  }
};
Point operator+(const Point &p1, const Point &p2) { return {p1.x + p2.x, p1.y + p2.y}; }

using PointSet = std::unordered_set<Point, PointHasher>;
using PointMap = std::unordered_map<Point, Point, PointHasher>;

constexpr std::array<Point, 8> DIRECTION = {{
    {0, -1}, // N
    {1, -1}, // NE
    {1, 0},  // E
    {1, 1},  // SE
    {0, 1},  // S
    {-1, 1}, // SW
    {-1, 0}, // W
    {-1, -1} // NW
}};

PointSet read_elf_coords(std::istream *input) {
  PointSet coords;
  coords.reserve(70 * 70);
  int y{0};
  for (std::string line; std::getline(*input, line);) {
    int x{0};
    for (const auto &ch : line) {
      switch (ch) {
      case '.':
        break;
      case '#':
        coords.insert({x, y});
        break;
      default:
        throw std::runtime_error("invalid character in input file");
      }
      ++x;
    }
    ++y;
  }
  return coords;
}

bool elf_has_neighbour(const Point &elf_point, const PointSet &elves) {
  for (const auto &direction : DIRECTION) {
    if (elves.contains(direction + elf_point)) {
      return true;
    }
  }
  return false;
}

std::pair<PointMap, std::unordered_map<Point, std::size_t, PointHasher>>
propose_moves(const PointSet &elves, const std::array<int, 4> &direction_priorities) {
  PointMap proposed_moves;
  proposed_moves.reserve(elves.size());
  std::unordered_map<Point, std::size_t, PointHasher> destination_counter;
  destination_counter.reserve(elves.size());
  for (const auto &elf_pos : elves) {
    if (!elf_has_neighbour(elf_pos, elves)) {
      continue;
    }

    for (const auto &direction : direction_priorities) {
      if (!(elves.contains(
                elf_pos +
                DIRECTION[direction == NORTH ? DIRECTION.size() - 1 : direction - 1]) ||
            elves.contains(elf_pos + DIRECTION[direction]) ||
            elves.contains(elf_pos + DIRECTION[(direction + 1) % 8]))) {
        auto dest = elf_pos + DIRECTION[direction];
        proposed_moves[elf_pos] = dest;
        destination_counter[dest] += 1;
        break;
      }
    }
  }
  return {proposed_moves, destination_counter};
}

int day23_1(std::istream *input_file) {
  auto elf_coords = read_elf_coords(input_file);

  std::size_t n_rounds{10};
  std::array<int, 4> direction_priority = {NORTH, SOUTH, WEST, EAST};

  for (auto i = 0U; i < n_rounds; ++i) {
    // Get the proposed moves and a count of how many elves are proposing to
    // move some position.
    auto [proposed_moves, dest_ctr] = propose_moves(elf_coords, direction_priority);

    // Move the elves if there are no collisions.
    PointSet new_elf_coords(elf_coords);
    for (const auto [src, dest] : proposed_moves) {
      if (dest_ctr[dest] == 1) {
        new_elf_coords.erase(src);
        new_elf_coords.insert(dest);
      }
    }
    elf_coords = new_elf_coords;

    // Rotate the move priorities.
    std::rotate(direction_priority.begin(), std::next(direction_priority.begin()),
                direction_priority.end());
  }

  // Find the bounds of the elf positions, make the smallest rectangle that
  // contains those bounds and find the number of empty points.
  std::array<int, 2> x_bounds{std::numeric_limits<int>::max(),
                              std::numeric_limits<int>::min()};
  std::array<int, 2> y_bounds{x_bounds};
  for (const auto &p : elf_coords) {
    x_bounds[0] = std::min(x_bounds[0], p.x);
    x_bounds[1] = std::max(x_bounds[1], p.x);
    y_bounds[0] = std::min(y_bounds[0], p.y);
    y_bounds[1] = std::max(y_bounds[1], p.y);
  }
  auto area = (x_bounds[1] - x_bounds[0] + 1) * (y_bounds[1] - y_bounds[0] + 1);
  return area - elf_coords.size();
}

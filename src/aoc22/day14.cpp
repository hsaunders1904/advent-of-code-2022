#include "aoc22/utils.h"

#include <array>
#include <istream>
#include <set>
#include <unordered_map>

namespace {
std::vector<int> range(int a, int b) {
  if (a == b) {
    return {a};
  }
  if (b > a) {
    std::swap(a, b);
  }
  std::vector<int> v;
  v.reserve(a - b);
  for (; b <= a; ++b) {
    v.emplace_back(b);
  }
  return v;
}

std::array<int, 2> to_point(const std::string &str) {
  const auto parts = split(str, ',');
  return {std::stoi(parts[0]) - 500, std::stoi(parts[1])};
}

std::unordered_map<int, std::set<int>> map_rocks(std::istream *input) {
  std::unordered_map<int, std::set<int>> rocks;
  for (std::string line; std::getline(*input, line);) {
    auto turning_points = split(line, " -> ");
    auto point_from = to_point(turning_points[0]);
    for (auto i = 1U; i < turning_points.size(); ++i) {
      auto point_to = to_point(turning_points[i]);
      if (point_to[0] == point_from[0]) {
        // Vertical
        for (auto row : range(point_to[1], point_from[1])) {
          rocks[row].insert(point_to[0]);
        }
      } else {
        // Horizontal
        for (auto col : range(point_to[0], point_from[0])) {
          rocks[point_to[1]].insert(col);
        }
      }
      point_from = point_to;
    }
  }
  return rocks;
}

bool is_filled(const std::unordered_map<int, std::set<int>> &waterfall, const int depth,
               const int col) {
  return waterfall.find(depth) != waterfall.end() &&
         waterfall.at(depth).find(col) != waterfall.at(depth).end();
}

template <typename T> int max_key(const std::unordered_map<int, T> &map) {
  return std::max_element(map.begin(), map.end(),
                          [](const auto &a, const auto &b) { return a.first < b.first; })
      ->first;
}

} // namespace

int day14_1(std::istream *input_file) {
  auto waterfall = map_rocks(input_file);

  int depth{0};
  const int max_depth = max_key(waterfall);
  int col{0};
  int fixed_sand{0};
  while (true) {
    if (is_filled(waterfall, depth + 1, col)) {
      if (is_filled(waterfall, depth + 1, col - 1)) {
        if (is_filled(waterfall, depth + 1, col + 1)) {
          waterfall[depth].insert(col);
          depth = 0;
          col = 0;
          ++fixed_sand;
        } else {
          ++depth;
          ++col;
        }
      } else {
        ++depth;
        --col;
      }
    } else {
      ++depth;
    }
    if (depth > max_depth) {
      break;
    }
  }
  return fixed_sand;
}

int day14_2(std::istream *input_file) {
  auto waterfall = map_rocks(input_file);

  int depth{0};
  const int max_depth = max_key(waterfall) + 2;
  int col{0};
  int fixed_sand{0};
  while (true) {
    if (depth + 1 >= max_depth) {
      waterfall[depth].insert(col);
      depth = 0;
      col = 0;
      ++fixed_sand;
    }
    if (is_filled(waterfall, depth + 1, col)) {
      if (is_filled(waterfall, depth + 1, col - 1)) {
        if (is_filled(waterfall, depth + 1, col + 1)) {
          ++fixed_sand;
          if (depth == 0) {
            break;
          }
          waterfall[depth].insert(col);
          depth = 0;
          col = 0;
        } else {
          ++depth;
          ++col;
        }
      } else {
        ++depth;
        --col;
      }
    } else {
      ++depth;
    }
  }
  return fixed_sand;
}

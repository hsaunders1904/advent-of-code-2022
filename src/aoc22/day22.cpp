#include "aoc22/utils.h"

#include <iostream>
#include <istream>

namespace {
std::pair<std::vector<std::string>, std::string> read_input(std::istream *input_file) {
  std::vector<std::string> lines;
  for (std::string line; std::getline(*input_file, line);) {
    lines.emplace_back(line);
  }
  auto instructions = lines.back();
  lines.pop_back();
  lines.pop_back();
  return {lines, instructions};
}

struct Map {
  Map(std::vector<std::string> map);
  void move(int magnitude, int dimension);
  const std::array<std::size_t, 2> &pos() const { return m_pos; };

private:
  std::string current_column() const;
  std::array<std::size_t, 2> m_pos = {0, 0};
  std::vector<std::string> m_map;
};

Map::Map(std::vector<std::string> map) : m_map(std::move(map)) {
  for (auto i = 0U; i < m_map.at(0).size(); ++i) {
    if (m_map[0][i] == '.') {
      m_pos[0] = i;
      return;
    }
  }
}

void Map::move(const int magnitude, const int dimension) {
  // Retrieve the row or column we're moving along
  auto line = (dimension == 0) ? m_map.at(pos()[1]) : current_column();

  const int direction = (0 < magnitude) - (magnitude < 0); // sign of the magnitude
  int dx{1};
  std::size_t last_non_empty = pos()[dimension];
  for (auto i = 0; i < std::abs(magnitude); ++i) {
    const auto next_idx =
        euclidean_remainder(pos()[dimension] + direction * dx, line.size());
    const auto next = line[next_idx];
    if (next == ' ') {
      ++dx;
      --i;
      continue;
    }
    if (next == '#') {
      break;
    }
    last_non_empty = next_idx;
    ++dx;
  }
  m_pos[dimension] = last_non_empty;
}

std::string Map::current_column() const {
  std::string col;
  for (auto row : m_map) {
    auto x = pos()[0];
    col += (x >= row.size()) ? ' ' : row[x];
  }
  return col;
}
} // namespace

int day22_1(std::istream *input_file) {
  const auto [map_str, instructions] = read_input(input_file);
  Map map(map_str);
  int direction = 0;

  for (auto i = 0U; i < instructions.size(); ++i) {
    std::string digits = "";
    while (i < instructions.size() && std::isdigit(instructions[i])) {
      digits += instructions[i];
      ++i;
    }
    int magnitude = (direction < 2) ? std::stoi(digits) : -std::stoi(digits);

    map.move(magnitude, direction % 2);

    if (i < instructions.size() && instructions[i] == 'R') {
      direction = euclidean_remainder(direction + 1, 4);
    } else if (instructions[i] == 'L') {
      direction = euclidean_remainder(direction - 1, 4);
    }
  }
  return 1000 * (map.pos()[1] + 1) + 4 * (map.pos()[0] + 1) + direction;
}

int day22_2(std::istream *input_file) { return 0; }

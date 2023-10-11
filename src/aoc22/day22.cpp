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

  void move_right(std::size_t magnitude);
  void move_left(std::size_t magnitude);
  void move_down(std::size_t magnitude);
  void move_up(std::size_t magnitude);
  std::array<std::size_t, 2> pos() const { return m_pos; };

  friend std::ostream &operator<<(std::ostream &os, const Map &map);

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

std::ostream &operator<<(std::ostream &os, const Map &map) {
  for (auto y = 0U; y < map.m_map.size(); ++y) {
    auto row = map.m_map[y];
    for (auto x = 0U; x < row.size(); ++x) {
      if (x == map.pos()[0] && y == map.pos()[1]) {
        os << 'X';
      } else {
        os << row[x];
      }
    }
    os << "\n";
  }
  return os;
}

void Map::move_right(const std::size_t magnitude) {
  std::size_t num_moved{0};
  int dx{1};
  std::size_t last_non_empty = pos()[0];
  auto row = m_map.at(pos()[1]);
  while (num_moved < magnitude) {
    auto next_idx = (pos()[0] + dx) % row.size();
    auto next = row[next_idx];
    if (next == ' ') {
      ++dx;
      continue;
    }
    if (next == '#') {
      break;
    }
    ++num_moved;
    last_non_empty = next_idx;
    ++dx;
  }
  m_pos[0] = last_non_empty;
}

void Map::move_left(const std::size_t magnitude) {
  std::size_t num_moved{0};
  int dx{1};
  std::size_t last_non_empty = pos()[0];
  auto row = m_map.at(pos()[1]);
  while (num_moved < magnitude) {
    auto next_idx = euclidean_remainder(pos()[0] - dx, row.size());
    auto next = row[next_idx];
    if (next == ' ') {
      ++dx;
      continue;
    }
    if (next == '#') {
      break;
    }
    ++num_moved;
    last_non_empty = next_idx;
    ++dx;
  }
  m_pos[0] = last_non_empty;
}

void Map::move_down(const std::size_t magnitude) {
  std::size_t num_moved{0};
  int dy{1};
  std::size_t last_non_empty = pos()[1];
  auto col = current_column();
  while (num_moved < magnitude) {
    auto next_idx = (pos()[1] + dy) % col.size();
    auto next = col[next_idx];
    if (next == ' ') {
      ++dy;
      continue;
    }
    if (next == '#') {
      break;
    }
    ++num_moved;
    last_non_empty = next_idx;
    ++dy;
  }
  m_pos[1] = last_non_empty;
}

void Map::move_up(const std::size_t magnitude) {
  std::size_t num_moved{0};
  int dy{1};
  std::size_t last_non_empty = pos()[1];
  auto col = current_column();
  while (num_moved < magnitude) {
    auto next_idx = euclidean_remainder(pos()[1] - dy, col.size());
    auto next = col[next_idx];
    if (next == ' ') {
      ++dy;
      continue;
    }
    if (next == '#') {
      break;
    }
    ++num_moved;
    last_non_empty = next_idx;
    ++dy;
  }
  m_pos[1] = last_non_empty;
}

std::string Map::current_column() const {
  std::string col;
  for (auto row : m_map) {
    auto x = pos()[0];
    if (x >= row.size()) {
      col += ' ';
    } else {
      col += row[x];
    }
  }
  return col;
}
} // namespace

int day22_1(std::istream *input_file) {
  const auto [map_str, instructions] = read_input(input_file);

  Map map(map_str);

  int direction = 0;
  std::string digit;
  std::size_t i{0};
  while (i < instructions.size()) {
    while (std::isdigit(instructions[i]) && i < instructions.size()) {
      digit += instructions[i];
      ++i;
    }
    std::size_t magnitude = std::stoul(digit);
    digit.clear();

    switch (direction) {
    case 0:
      map.move_right(magnitude);
      break;
    case 1:
      map.move_down(magnitude);
      break;
    case 2:
      map.move_left(magnitude);
      break;
    case 3:
      map.move_up(magnitude);
      break;
    default:
      throw std::logic_error("invalid direction");
    }

    if (!std::isdigit(instructions[i])) {
      if (instructions[i] == 'R') {
        direction += 1;
      } else if (instructions[i] == 'L') {
        direction -= 1;
      }
      direction = euclidean_remainder(direction, 4);
    }
    ++i;
  }
  return 1000 * (map.pos()[1] + 1) + 4 * (map.pos()[0] + 1) + direction;
}

int day22_2(std::istream *input_file) { return 0; }

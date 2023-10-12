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

constexpr std::size_t EAST{0};
constexpr std::size_t SOUTH{1};
constexpr std::size_t WEST{2};
constexpr std::size_t NORTH{3};

struct CubeMap {
  CubeMap(std::vector<std::string> map_vec, std::string directions_str)
      : map(std::move(map_vec)), directions(std::move(directions_str)) {
    for (auto i = 0U; i < map.size(); ++i) {
      if (map[0][i] == '.') {
        x = static_cast<int>(i);
        break;
      }
    }
    width = map.size() / 4;
  }

  void move(int distance);
  std::array<int, 3> next_east() const;
  std::array<int, 3> next_south() const;
  std::array<int, 3> next_west() const;
  std::array<int, 3> next_north() const;

  std::vector<std::string> map;
  std::string directions;
  int direction = EAST;
  int x{0};
  int y{0};
  int width;
};

bool is_between(const int v, const int min, const int max) {
  return min <= v && v < max;
}

std::array<int, 3> CubeMap::next_east() const {
  if (is_between(y, 0, width) && x == 3 * width - 1) {
    return {2 * width - 1, 3 * width - y - 1, WEST};
  } else if (is_between(y, width, 2 * width) && x == 2 * width - 1) {
    return {width + y, width - 1, NORTH};
  } else if (is_between(y, 2 * width, 3 * width) && x == 2 * width - 1) {
    return {3 * width - 1, 3 * width - y - 1, WEST};
  } else if (is_between(y, 3 * width, 4 * width) && x == width - 1) {
    return {2 * width - (4 * width - y), 3 * width - 1, NORTH};
  }
  return {x + 1, y, EAST};
}

std::array<int, 3> CubeMap::next_south() const {
  if (is_between(x, 0, width) && y == 4 * width - 1) {
    return {2 * width + x, 0, SOUTH};
  } else if (is_between(x, width, 2 * width) && y == 3 * width - 1) {
    return {width - 1, 4 * width - (2 * width - x), WEST};
  } else if (is_between(x, 2 * width, 3 * width) && y == width - 1) {
    return {2 * width - 1, 2 * width - (3 * width - x), WEST};
  }
  return {x, y + 1, SOUTH};
}

std::array<int, 3> CubeMap::next_west() const {
  if (is_between(y, 0, width) && x == width) {
    return {0, 3 * width - 1 - y, EAST};
  } else if (is_between(y, width, 2 * width) && x == width) {
    return {y - width, 2 * width, SOUTH};
  } else if (is_between(y, 2 * width, 3 * width) && x == 0) {
    return {width, width - 1 - (y - 2 * width), EAST};
  } else if (is_between(y, 3 * width, 4 * width) && x == 0) {
    return {width + (y - 3 * width), 0, SOUTH};
  }
  return {x - 1, y, WEST};
}

std::array<int, 3> CubeMap::next_north() const {
  if (is_between(x, 0, width) && y == 2 * width) {
    return {width, width + x, EAST};
  } else if (is_between(x, width, 2 * width) && y == 0) {
    return {0, 3 * width + (x - width), EAST};
  } else if (is_between(x, 2 * width, 3 * width) && y == 0) {
    return {x - 2 * width, 4 * width - 1, NORTH};
  }
  return {x, y - 1, NORTH};
}

void CubeMap::move(const int distance) {
  for (auto i = 0; i < distance; ++i) {
    std::array<int, 3> new_state;
    switch (direction) {
    case EAST:
      new_state = next_east();
      break;
    case SOUTH:
      new_state = next_south();
      break;
    case WEST:
      new_state = next_west();
      break;
    case NORTH:
      new_state = next_north();
      break;
    default:
      throw std::runtime_error("invalid direction");
    }

    if (map.at(new_state[1]).at(new_state[0]) == '.') {
      x = new_state[0];
      y = new_state[1];
      direction = new_state[2];
    }
  }
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

int day22_2(std::istream *input_file) {
  const auto [map_str, instructions] = read_input(input_file);
  CubeMap map(map_str, instructions);
  for (auto i = 0U; i < instructions.size(); ++i) {
    std::string digits = "";
    while (i < instructions.size() && std::isdigit(instructions[i])) {
      digits += instructions[i];
      ++i;
    }
    int magnitude = std::stoi(digits);

    map.move(magnitude);

    if (i < instructions.size() && instructions[i] == 'R') {
      map.direction = euclidean_remainder(map.direction + 1, 4);
    } else if (instructions[i] == 'L') {
      map.direction = euclidean_remainder(map.direction - 1, 4);
    }
  }
  return 1000 * (map.y + 1) + 4 * (map.x + 1) + map.direction;
}

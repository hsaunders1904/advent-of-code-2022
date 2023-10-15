#include "aoc22/utils.h"

#include <iostream>
#include <istream>

namespace {
constexpr std::size_t EAST{0};
constexpr std::size_t SOUTH{1};
constexpr std::size_t WEST{2};
constexpr std::size_t NORTH{3};

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
  virtual void move(int magnitude);
  int x{0};
  int y{0};
  int direction = EAST;

protected:
  std::string current_column() const;
  std::vector<std::string> m_map;
};

Map::Map(std::vector<std::string> map) : m_map(std::move(map)) {
  for (auto i = 0U; i < m_map.at(0).size(); ++i) {
    if (m_map[0][i] == '.') {
      x = i;
      return;
    }
  }
}

void Map::move(const int magnitude) {
  // Retrieve the row or column we're moving along
  int dimension = direction % 2;
  auto line = (dimension == 0) ? m_map.at(y) : current_column();

  const int step = direction < 2 ? 1 : -1;
  int dx{1};
  std::size_t start = dimension == 0 ? x : y;
  std::size_t last_non_empty = start;
  for (auto i = 0; i < std::abs(magnitude); ++i) {
    const auto next_idx = euclidean_remainder(start + step * dx, line.size());
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
  if (dimension == 0) {
    x = last_non_empty;
  } else {
    y = last_non_empty;
  }
}

std::string Map::current_column() const {
  std::string col;
  for (auto row : m_map) {
    col += (static_cast<std::size_t>(x) >= row.size()) ? ' ' : row[x];
  }
  return col;
}

void apply_instructions(Map *map, const std::string &instructions) {
  for (auto i = 0U; i < instructions.size(); ++i) {
    std::string digits = "";
    while (i < instructions.size() && std::isdigit(instructions[i])) {
      digits += instructions[i];
      ++i;
    }
    int magnitude = std::stoi(digits);

    map->move(magnitude);

    if (i < instructions.size()) {
      if (instructions[i] == 'R') {
        map->direction = euclidean_remainder(map->direction + 1, 4);
      } else if (instructions[i] == 'L') {
        map->direction = euclidean_remainder(map->direction - 1, 4);
      }
    }
  }
}
} // namespace

int day22_1(std::istream *input_file) {
  const auto [map_str, instructions] = read_input(input_file);
  Map map(map_str);
  apply_instructions(&map, instructions);
  return 1000 * (map.y + 1) + 4 * (map.x + 1) + map.direction;
}

namespace {
/*
Our input looks like this:

               5
+--> x   4 +---+---+ 2
|          |   |   |
v y      1 +---+---+ 3
           |   |
     1 +---+---+ 3
       |   |   |
     4 +---+---+ 2
       |   |
     5 +---+ 2

I've labelled the vertices that give the corners of the cube when the map is folded.
The edges with the same two vertices will teleport to each other.
The 'teleportation' cases are manually written out in the 'next_' member functions.
This solution will work for any size map, but it must have the same shape as the above
diagram.
*/
struct CubeMap : public Map {
  CubeMap(std::vector<std::string> map_vec) : Map(map_vec) { width = m_map.size() / 4; }
  void move(int magnitude) override;
  int width;

protected:
  std::array<int, 3> next_east() const;
  std::array<int, 3> next_south() const;
  std::array<int, 3> next_west() const;
  std::array<int, 3> next_north() const;
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
    return {y - 2 * width, 3 * width - 1, NORTH};
  }
  return {x + 1, y, EAST};
}

std::array<int, 3> CubeMap::next_south() const {
  if (is_between(x, 0, width) && y == 4 * width - 1) {
    return {2 * width + x, 0, SOUTH};
  } else if (is_between(x, width, 2 * width) && y == 3 * width - 1) {
    return {width - 1, 2 * width + x, WEST};
  } else if (is_between(x, 2 * width, 3 * width) && y == width - 1) {
    return {2 * width - 1, x - width, WEST};
  }
  return {x, y + 1, SOUTH};
}

std::array<int, 3> CubeMap::next_west() const {
  if (is_between(y, 0, width) && x == width) {
    return {0, 3 * width - 1 - y, EAST};
  } else if (is_between(y, width, 2 * width) && x == width) {
    return {y - width, 2 * width, SOUTH};
  } else if (is_between(y, 2 * width, 3 * width) && x == 0) {
    return {width, 3 * width - y - 1, EAST};
  } else if (is_between(y, 3 * width, 4 * width) && x == 0) {
    return {y - 2 * width, 0, SOUTH};
  }
  return {x - 1, y, WEST};
}

std::array<int, 3> CubeMap::next_north() const {
  if (is_between(x, 0, width) && y == 2 * width) {
    return {width, width + x, EAST};
  } else if (is_between(x, width, 2 * width) && y == 0) {
    return {0, 2 * width + x, EAST};
  } else if (is_between(x, 2 * width, 3 * width) && y == 0) {
    return {x - 2 * width, 4 * width - 1, NORTH};
  }
  return {x, y - 1, NORTH};
}

void CubeMap::move(const int magnitude) {
  for (auto i = 0; i < magnitude; ++i) {
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
    if (m_map.at(new_state[1]).at(new_state[0]) == '#') {
      break;
    }
    x = new_state[0];
    y = new_state[1];
    direction = new_state[2];
  }
}
} // namespace

int day22_2(std::istream *input_file) {
  const auto [map_str, instructions] = read_input(input_file);
  CubeMap map(map_str);
  apply_instructions(&map, instructions);
  return 1000 * (map.y + 1) + 4 * (map.x + 1) + map.direction;
}

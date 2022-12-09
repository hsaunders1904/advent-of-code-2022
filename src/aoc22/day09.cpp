#include "aoc22/aoc.h"

#include <array>
#include <cmath>
#include <unordered_set>

namespace {
const double MAX_DIST = std::sqrt(2);

std::pair<char, int> parse_instruction(const std::string &line) {
  auto parts = split(line, ' ');
  char direction = parts[0][0];
  auto magnitude = std::stoi(parts[1]);
  return {direction, magnitude};
}

void update_pos(std::array<int, 2> *pos, const char direction) {
  switch (direction) {
  case 'R':
    (*pos)[0] += 1;
    break;
  case 'L':
    (*pos)[0] -= 1;
    break;
  case 'U':
    (*pos)[1] += 1;
    break;
  case 'D':
    (*pos)[1] -= 1;
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
}

double distance(const std::array<int, 2> &h_pos,
                const std::array<int, 2> &t_pos) {
  auto dx = h_pos[0] - t_pos[0];
  auto dy = h_pos[1] - t_pos[1];
  return std::sqrt(dx * dx + dy * dy);
}

inline std::string encode_pos(const std::array<int, 2> &pos) {
  return std::to_string(pos[0]) + "," + std::to_string(pos[1]);
}
} // namespace

int day09_1(std::istream *input_file) {
  std::array<int, 2> h_pos{0};
  std::array<int, 2> t_pos{0};
  std::unordered_set<std::string> t_positions;
  t_positions.insert(encode_pos(t_pos));
  for (std::string line; std::getline(*input_file, line);) {
    if (line.empty()) {
      break;
    }
    auto [direction, magnitude] = parse_instruction(line);
    for (auto i = 0; i < magnitude; ++i) {
      auto old_h_pos = h_pos;
      update_pos(&h_pos, direction);
      if (distance(h_pos, t_pos) > MAX_DIST) {
        t_pos = old_h_pos;
      }
      t_positions.insert(encode_pos(t_pos));
    }
  }
  return t_positions.size();
}

int day09_2(std::istream *input_file) { return 1; }

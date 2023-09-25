#include "aoc22/utils.h"

#include <array>
#include <istream>
#include <unordered_set>

namespace {
std::pair<char, int> parse_instruction(const std::string &line) {
  auto parts = split(line, ' ');
  char direction = parts[0][0];
  auto magnitude = std::stoi(parts[1]);
  return {direction, magnitude};
}

void update_head(std::array<int, 2> *pos, const char direction) {
  switch (direction) {
  case 'R':
    pos->at(0) += 1;
    break;
  case 'L':
    pos->at(0) -= 1;
    break;
  case 'U':
    pos->at(1) += 1;
    break;
  case 'D':
    pos->at(1) -= 1;
    break;
  default:
    throw std::runtime_error("Invalid direction");
  }
}

inline uint64_t encode_pos(const std::array<int, 2> &pos) {
  // Store x in the first 32 bits and y in the final 32, to give us a unique
  // 64-bit integer we can insert into a set
  constexpr uint64_t size_of_int{32};
  return static_cast<uint64_t>(static_cast<uint32_t>(pos[0])) << size_of_int |
         static_cast<uint32_t>(pos[1]);
}

void update_tail(std::array<int, 2> *tail, const std::array<int, 2> &head) {
  auto dx = head[0] - tail->at(0);
  auto dy = head[1] - tail->at(1);
  if (dx > 0) {
    dx--;
  } else if (dx < 0) {
    dx++;
  }
  if (dy > 0) {
    dy--;
  } else if (dy < 0) {
    dy++;
  }
  if (dx != 0 || dy != 0) {
    *tail = {head[0] - dx, head[1] - dy};
  }
}

int count_tail_positions(std::istream *input_file, const std::size_t num_knots) {
  std::vector<std::array<int, 2>> knots(num_knots);
  std::unordered_set<uint64_t> t_positions({encode_pos({0, 0})});
  for (std::string line; std::getline(*input_file, line);) {
    auto [direction, magnitude] = parse_instruction(line);
    for (auto i = 0; i < magnitude; ++i) {
      update_head(&knots[0], direction);
      for (auto k = 0U; k < knots.size() - 1; ++k) {
        update_tail(&knots[k + 1], knots[k]);
      }
      t_positions.insert(encode_pos(knots[knots.size() - 1]));
    }
  }
  return t_positions.size();
}
} // namespace

int day09_1(std::istream *input_file) { return count_tail_positions(input_file, 2); }

int day09_2(std::istream *input_file) { return count_tail_positions(input_file, 10); }

#include "aoc22/aoc.h"

#include <array>

namespace {
constexpr auto ADDX = "addx";

std::pair<std::string, int> parse_instruction(const std::string &line) {
  auto parts = split(line, ' ');
  if (parts.size() > 1) {
    return {parts[0], std::stoi(parts[1])};
  }
  return {"", 0};
}
} // namespace

int day10_1(std::istream *input_file) {
  int x_reg{1};
  std::size_t cycle_num{1};
  int signal_strength_sum{0};
  for (std::string line; std::getline(*input_file, line); ++cycle_num) {
    auto [instruction, value] = parse_instruction(line);
    if ((cycle_num + 20) % 40 == 0) {
      signal_strength_sum += x_reg * cycle_num;
    }
    if (instruction == ADDX) {
      cycle_num++;
      if ((cycle_num + 20) % 40 == 0) {
        signal_strength_sum += x_reg * cycle_num;
      }
      x_reg += value;
    }
  }

  return signal_strength_sum;
}

int day10_2(std::istream *input_file) { return 1; }


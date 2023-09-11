#include "aoc22/aoc.h"

namespace {
constexpr auto ADDX = "addx";
constexpr auto DISPLAY_WIDTH{40};

std::pair<std::string, int> parse_instruction(const std::string &line) {
  auto parts = split(line, ' ');
  if (parts.size() > 1) {
    return {parts[0], std::stoi(parts[1])};
  }
  return {"", 0};
}

void update_signal(int *signal, const int cycle_num, const int x_reg) {
  if ((cycle_num + 20) % DISPLAY_WIDTH == 0) {
    *signal += x_reg * cycle_num;
  }
}

void update_display(std::string *display, const int cycle_num,
                    const int x_reg) {
  auto pixel_pos = (cycle_num - 1) % DISPLAY_WIDTH;
  if (pixel_pos == 0 && cycle_num != 1) {
    *display += "\n";
  }
  if (pixel_pos >= x_reg - 1 && pixel_pos <= x_reg + 1) {
    *display += "#";
  } else {
    *display += ".";
  }
}

template <typename T>
T iter_instructions(std::istream *input_file,
                    std::function<void(T *, int, int)> accumulator) {
  int x_reg{1};
  int cycle_num{1};
  T acc{};
  for (std::string line; std::getline(*input_file, line); ++cycle_num) {
    auto [instruction, value] = parse_instruction(line);
    accumulator(&acc, cycle_num, x_reg);
    if (instruction == ADDX) {
      cycle_num++;
      accumulator(&acc, cycle_num, x_reg);
      x_reg += value;
    }
  }
  return acc;
}
} // namespace

int day10_1(std::istream *input_file) {
  return iter_instructions<int>(input_file, update_signal);
}

std::string day10_2(std::istream *input_file) {
  return iter_instructions<std::string>(input_file, update_display) + "\n";
}

#include "aoc22/aoc.h"

int day03_1(std::istream *input_file) {
  int priority_sum{0};
  for (std::string line; std::getline(*input_file, line);) {
    for (auto i = 0U; i < line.size() / 2; ++i) {
      if (line.find_first_of(line[i], line.size() / 2) != std::string::npos) {
        if (line[i] <= 122 && line[i] >= 97) {
          priority_sum += line[i] - 96;
        } else {
          priority_sum += line[i] - 38;
        }
        break;
      }
    }
  }
  return priority_sum;
}

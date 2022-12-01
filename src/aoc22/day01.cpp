#include "aoc22/aoc.h"

int day01(std::istream *input_file) {
  std::size_t max_count{0};
  std::size_t current_count{0};
  for (std::string line; std::getline(*input_file, line);) {
    if (line.empty()) {
      if (current_count > max_count) {
        max_count = current_count;
      }
      current_count = 0;
    } else {
      current_count += std::stoi(line);
    }
  }
  return max_count;
}

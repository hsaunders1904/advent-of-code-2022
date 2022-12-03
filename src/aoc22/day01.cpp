#include "aoc22/aoc.h"

#include <algorithm>
#include <numeric>
#include <vector>

int day01_1(std::istream *input_file) {
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

int day01_2(std::istream *input_file) {
  std::vector<std::size_t> counts;
  std::size_t current_count{0};
  for (std::string line; std::getline(*input_file, line);) {
    if (line.empty()) {
      counts.emplace_back(current_count);
      current_count = 0;
    } else {
      current_count += std::stoi(line);
    }
  }
  std::partial_sort(counts.begin(), std::next(counts.begin(), 3), counts.end(),
                    std::greater<>());
  return std::accumulate(counts.begin(), std::next(counts.begin(), 3), 0);
}

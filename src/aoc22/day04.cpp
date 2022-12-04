#include "aoc22/aoc.h"

#include <array>

namespace {
std::pair<std::string, std::string> split(const std::string &str,
                                          const char split_str) {
  auto split_pos = str.find(split_str);
  auto first = str.substr(0, split_pos);
  auto second = str.substr(split_pos + 1, str.size());
  return {first, second};
}

std::array<int, 4> parse_ranges(const std::string &line) {
  auto [range_1, range_2] = split(line, ',');
  auto [v0, v1] = split(range_1, '-');
  auto [v2, v3] = split(range_2, '-');

  std::array<int, 4> values;
  auto i = 0U;
  for (const auto &el : {v0, v1, v2, v3}) {
    values[i] = std::stoi(el);
    ++i;
  }
  return values;
}
} // namespace

int day04_1(std::istream *input_file) {
  int fully_contained_pairs{0};
  for (std::string line; std::getline(*input_file, line);) {
    auto ranges = parse_ranges(line);
    if (ranges[0] <= ranges[2] && ranges[1] >= ranges[3]) {
      fully_contained_pairs += 1;
    } else if (ranges[2] <= ranges[0] && ranges[3] >= ranges[1]) {
      fully_contained_pairs += 1;
    }
  }
  return fully_contained_pairs;
}

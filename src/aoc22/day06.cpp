#include "aoc22/aoc.h"

#include <set>

namespace {
int find_distinct_char_idx(const std::string &signal, std::size_t window_size) {
  auto window_beg = signal.begin();
  auto window_end = std::next(window_beg, window_size);
  for (auto i = 0; i < signal.size() - window_size; ++i) {
    std::set<char> window(window_beg++, window_end++);
    if (window.size() == window_size) {
      return i + window_size;
    }
  }
  return -1;
}
} // namespace

int day06_1(std::istream *input_file) {
  std::string signal;
  std::getline(*input_file, signal);
  return find_distinct_char_idx(signal, 4);
}

int day06_2(std::istream *input_file) {
  std::string signal;
  std::getline(*input_file, signal);
  return find_distinct_char_idx(signal, 14);
}

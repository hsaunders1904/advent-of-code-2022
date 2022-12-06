#include "aoc22/aoc.h"

namespace {
bool has_no_duplicates(const std::array<int, 26> &counts,
                       const std::string &signal, std::size_t start_idx,
                       std::size_t window_size) {

  auto it = std::next(signal.begin(), start_idx);
  return std::all_of(it, std::next(it, window_size),
                     [&counts](auto ch) { return counts[ch - 'a'] == 1; });
}

int find_distinct_char_idx(const std::string &signal, std::size_t window_size) {
  std::array<int, 26> char_counts{0};
  for (auto i = 0U; i < window_size; ++i) {
    char_counts[signal[i] - 'a']++;
  }
  if (has_no_duplicates(char_counts, signal, window_size, window_size)) {
    return window_size;
  }
  for (auto i = window_size; i < signal.size(); ++i) {
    char_counts[signal[i - window_size] - 'a']--;
    char_counts[signal[i] - 'a']++;
    if (has_no_duplicates(char_counts, signal, i - window_size + 1,
                          window_size)) {
      return i + 1;
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

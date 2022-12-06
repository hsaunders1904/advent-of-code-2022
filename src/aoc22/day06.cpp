#include "aoc22/aoc.h"

namespace {
bool has_no_duplicates(const std::array<int, 26> &counts,
                       const std::string &signal, std::size_t start_idx) {

  auto it = std::next(signal.begin(), start_idx);
  return std::all_of(it, std::next(it, 4),
                     [&counts](auto ch) { return counts[ch - 'a'] == 1; });
}
} // namespace

int day06_1(std::istream *input_file) {
  std::string signal;
  std::getline(*input_file, signal);

  std::array<int, 26> char_counts{0};
  for (auto i = 0U; i < 4; ++i) {
    char_counts[signal[i] - 'a']++;
  }
  if (has_no_duplicates(char_counts, signal, 4)) {
    return 4;
  }
  for (auto i = 4U; i < signal.size(); ++i) {
    char_counts[signal[i - 4] - 'a']--;
    char_counts[signal[i] - 'a']++;
    if (has_no_duplicates(char_counts, signal, i - 3)) {
      return i + 1;
    }
  }
  return -1;
}

int day06_2(std::istream *input_file) { return 0; }
#include "aoc22/aoc.h"

#include <numeric>

namespace {
std::tuple<std::vector<std::size_t>, std::size_t>
read_matrix(std::istream *input) {
  std::vector<std::size_t> heights;
  std::size_t width{0};
  for (std::string line; std::getline(*input, line);) {
    if (line.empty()) {
      break;
    }
    width = line.size();
    for (const char ch : line) {
      heights.emplace_back(ch - '0');
    }
  }
  return {heights, width};
}

bool idx_visible(const std::vector<std::size_t> &mat, std::size_t row,
                 std::size_t col, std::size_t width) {
  const auto el = mat[row * width + col];

  auto visible_left = true;
  for (auto i = 0U; i < row; ++i) {
    auto other = mat[i * width + col];
    if (other >= el) {
      visible_left = false;
      break;
    }
  }
  if (visible_left) {
    return true;
  }

  auto visible_right = true;
  for (auto i = row + 1; i < width; ++i) {
    auto other = mat[i * width + col];
    if (other >= el) {
      visible_right = false;
      break;
    }
  }
  if (visible_right) {
    return true;
  }

  auto visible_up = true;
  for (auto i = 0U; i < col; ++i) {
    auto other = mat[row * width + i];
    if (other >= el) {
      visible_up = false;
      break;
    }
  }
  if (visible_up) {
    return true;
  }

  auto visible_down = true;
  for (auto i = col + 1; i < width; ++i) {
    auto other = mat[row * width + i];
    if (other >= el) {
      visible_down = false;
      break;
    }
  }
  if (visible_down) {
    return true;
  }
  return false;
}
} // namespace

int day08_1(std::istream *input_file) {
  const auto [heights, width] = read_matrix(input_file);
  std::vector<bool> visible(heights.size(), true);
  for (auto row_idx = 1U; row_idx < width - 1; ++row_idx) {
    for (auto col_idx = 1U; col_idx < width - 1; ++col_idx) {
      visible[row_idx * width + col_idx] =
          idx_visible(heights, row_idx, col_idx, width);
    }
  }
  return std::accumulate(visible.begin(), visible.end(), 0);
}

int day08_2(std::istream *input_file) { return 1; }

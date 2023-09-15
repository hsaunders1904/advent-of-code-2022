#include <array>
#include <istream>
#include <numeric>

namespace {
std::tuple<std::vector<std::size_t>, std::size_t> read_matrix(std::istream *input) {
  std::vector<std::size_t> heights;
  std::size_t width{0};
  for (std::string line; std::getline(*input, line);) {
    if (line.empty()) {
      break;
    }
    width = line.size();
    std::transform(line.begin(), line.end(), std::back_inserter(heights),
                   [](auto ch) { return ch - '0'; });
  }
  return {heights, width};
}

bool idx_visible(const std::vector<std::size_t> &mat, const std::size_t row,
                 const std::size_t col, const std::size_t width) {
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

int idx_scenic_score(const std::vector<std::size_t> &mat, const std::size_t row,
                     const std::size_t col, const std::size_t width) {
  auto el = mat[row * width + col];
  std::array<int, 4> scores{0};

  // Look left
  for (std::size_t i = col - 1; i != static_cast<std::size_t>(-1); --i) {
    auto other = mat[row * width + i];
    scores[0] += 1;
    if (other >= el) {
      break;
    }
  }
  // Look right
  for (auto i = col + 1; i < width; ++i) {
    auto other = mat[row * width + i];
    scores[1] += 1;
    if (other >= el) {
      break;
    }
  }
  // Look up
  for (std::size_t i = row - 1; i != static_cast<std::size_t>(-1); --i) {
    auto other = mat[i * width + col];
    scores[2] += 1;
    if (other >= el) {
      break;
    }
  }
  // Look down
  for (auto i = row + 1; i < width; ++i) {
    auto other = mat[i * width + col];
    scores[3] += 1;
    if (other >= el) {
      break;
    }
  }
  return std::accumulate(scores.begin(), scores.end(), 1, std::multiplies<>());
}
} // namespace

int day08_1(std::istream *input_file) {
  const auto [heights, width] = read_matrix(input_file);
  int num_visible = 4 * (width - 1);
  for (auto row_idx = 1U; row_idx < width - 1; ++row_idx) {
    for (auto col_idx = 1U; col_idx < width - 1; ++col_idx) {
      num_visible += idx_visible(heights, row_idx, col_idx, width);
    }
  }
  return num_visible;
}

int day08_2(std::istream *input_file) {
  const auto [heights, width] = read_matrix(input_file);
  int max_scenic_score{0};
  for (auto row_idx = 1U; row_idx < width - 1; ++row_idx) {
    for (auto col_idx = 1U; col_idx < width - 1; ++col_idx) {
      auto score = idx_scenic_score(heights, row_idx, col_idx, width);
      if (score > max_scenic_score) {
        max_scenic_score = score;
      }
    }
  }
  return max_scenic_score;
}

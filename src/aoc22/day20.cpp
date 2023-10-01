#include <iostream>
#include <istream>
#include <list>

namespace {
std::vector<int> read_numbers(std::istream *input_file) {
  std::vector<int> numbers;
  for (std::string line; std::getline(*input_file, line);) {
    numbers.emplace_back(std::stoi(line));
  }
  return numbers;
}

template <typename T> std::vector<T> range(std::size_t n) {
  std::vector<std::size_t> vec;
  vec.reserve(n);
  for (auto i = 0U; i < n; ++i) {
    vec.emplace_back(i);
  }
  return vec;
}

int euclidean_remainder(const int a, const int b) {
  auto r = a % b;
  return r >= 0 ? r : r + std::abs(b);
}

std::pair<std::vector<int>, std::size_t> mix_numbers(const std::vector<int> &numbers) {
  // Create an array of indices that we'll mix, so it's easier to find the next number
  // value to shift (no duplicates and linear ordering).
  auto idxs = range<std::size_t>(numbers.size());

  for (auto number_id = 0U; number_id < numbers.size(); ++number_id) {
    // Remove the next element
    auto pos = std::find(idxs.begin(), idxs.end(), number_id);
    idxs.erase(pos);

    // Find where we need to put the removed element
    int shift_by = numbers.at(number_id);
    auto idx = std::distance(idxs.begin(), pos);
    auto new_idx = euclidean_remainder(idx + shift_by, idxs.size());

    // Put the element back into the list of indices
    if (new_idx == 0) {
      idxs.insert(idxs.end(), number_id);
    } else {
      idxs.insert(std::next(idxs.begin(), new_idx), number_id);
    }
  }

  // Mix the actual values using the mixed indices. Keep track of the position of the 0
  // for later
  std::size_t zero_pos{0};
  std::vector<int> mixed_numbers;
  mixed_numbers.reserve(numbers.size());
  for (auto i = 0U; i < idxs.size(); ++i) {
    auto num = numbers.at(idxs[i]);
    if (num == 0) {
      zero_pos = i;
    }
    mixed_numbers.emplace_back(num);
  }

  return {mixed_numbers, zero_pos};
}
} // namespace

int day20_1(std::istream *input_file) {
  auto numbers = read_numbers(input_file);
  const auto [mixed_numbers, zero_pos] = mix_numbers(numbers);

  return mixed_numbers[(zero_pos + 1000) % numbers.size()] +
         mixed_numbers[(zero_pos + 2000) % numbers.size()] +
         mixed_numbers[(zero_pos + 3000) % numbers.size()];
}

int day20_2(std::istream *input_file) { return 0; }

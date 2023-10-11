#include "aoc22/utils.h"

#include <iostream>
#include <istream>
#include <string>
#include <vector>

namespace {
std::vector<long> read_numbers(std::istream *input_file, const long multiplier = 1) {
  std::vector<long> numbers;
  for (std::string line; std::getline(*input_file, line);) {
    numbers.emplace_back(std::stoi(line) * multiplier);
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

void mix_indices(std::vector<std::size_t> *idxs, const std::vector<long> &numbers) {
  for (auto number_id = 0U; number_id < numbers.size(); ++number_id) {
    // Remove the next element
    auto pos = std::find(idxs->begin(), idxs->end(), number_id);
    idxs->erase(pos);

    // Find where we need to put the removed element
    long shift_by = numbers.at(number_id);
    auto idx = std::distance(idxs->begin(), pos);
    auto new_idx = euclidean_remainder(idx + shift_by, idxs->size());

    // Put the element back into the list of indices
    if (new_idx == 0) {
      idxs->insert(idxs->end(), number_id);
    } else {
      idxs->insert(std::next(idxs->begin(), new_idx), number_id);
    }
  }
}

std::pair<std::vector<long>, std::size_t>
mix_numbers(const std::vector<long> &numbers, const std::vector<std::size_t> &idxs) {
  std::size_t zero_pos{0};
  std::vector<long> mixed_numbers;
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

long sum_coordinates(const std::vector<long> &numbers, const std::size_t zero_pos) {
  return numbers[(zero_pos + 1000) % numbers.size()] +
         numbers[(zero_pos + 2000) % numbers.size()] +
         numbers[(zero_pos + 3000) % numbers.size()];
}
} // namespace

long day20_1(std::istream *input_file) {
  const auto numbers = read_numbers(input_file);
  // Create an array of indices that we'll mix, so it's easier to find the next number
  // value to shift (no duplicates and linear ordering)
  auto idxs = range<std::size_t>(numbers.size());
  mix_indices(&idxs, numbers);
  // Mix the actual numbers using our mixed indices
  const auto [mixed_numbers, zero_pos] = mix_numbers(numbers, idxs);
  return sum_coordinates(mixed_numbers, zero_pos);
}

long day20_2(std::istream *input_file) {
  constexpr std::size_t encryption_key{811'589'153};
  constexpr std::size_t num_repetitions{10};

  const auto numbers = read_numbers(input_file, encryption_key);
  auto idxs = range<std::size_t>(numbers.size());
  for (auto i = 0U; i < num_repetitions; ++i) {
    mix_indices(&idxs, numbers);
  }
  const auto [mixed_numbers, zero_pos] = mix_numbers(numbers, idxs);
  return sum_coordinates(mixed_numbers, zero_pos);
}

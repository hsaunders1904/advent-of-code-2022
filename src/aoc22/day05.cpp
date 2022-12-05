#include "aoc22/aoc.h"

#include <stack>

namespace {
void fill_row(std::vector<std::vector<char>> *stacks, const std::string &line) {
  for (auto i = 0U; i < stacks->size(); ++i) {
    if (line[1 + i * 4] != ' ') {
      (*stacks)[i].emplace_back(line[1 + i * 4]);
    }
  }
}

std::vector<std::stack<char>> read_stacks(std::istream *input_file) {
  std::string line;
  std::getline(*input_file, line);
  std::vector<std::vector<char>> vec_stacks((line.size() + 1) / 4);
  while (line[1] != '1') {
    fill_row(&vec_stacks, line);
    std::getline(*input_file, line);
  }
  std::getline(*input_file, line); // skip blank line

  std::vector<std::stack<char>> stacks(vec_stacks.size());
  for (auto i = 0U; i < vec_stacks.size(); ++i) {
    for (auto it = vec_stacks[i].rbegin(); it != vec_stacks[i].rend(); ++it) {
      stacks[i].push(*it);
    }
  }
  return stacks;
}

std::array<int, 3> parse_instruction(const std::string &line) {
  std::array<int, 3> instruction;
  const auto parts = split(line, ' ');
  return {std::stoi(parts.at(1)), std::stoi(parts.at(3)) - 1,
          std::stoi(parts.at(5)) - 1};
}
} // namespace

std::string day05_1(std::istream *input_file) {
  auto stacks = read_stacks(input_file);
  for (std::string line; std::getline(*input_file, line);) {
    const auto [num_to_move, from, to] = parse_instruction(line);
    for (auto i = 0U; i < num_to_move; ++i) {
      stacks[to].push(stacks[from].top());
      stacks[from].pop();
    }
  }
  return std::accumulate(
      stacks.begin(), stacks.end(), std::string{},
      [](const auto &acc, const auto &st) { return acc + st.top(); });
}

std::string day05_2(std::istream *input_file) {
  auto stacks = read_stacks(input_file);
  for (std::string line; std::getline(*input_file, line);) {
    const auto [num_to_move, from, to] = parse_instruction(line);
    std::vector<char> crates(num_to_move);
    for (auto i = 0U; i < crates.size(); ++i) {
      crates[crates.size() - 1 - i] = stacks[from].top();
      stacks[from].pop();
    }
    for (const auto &el : crates) {
      stacks[to].push(el);
    }
  }
  return std::accumulate(
      stacks.begin(), stacks.end(), std::string{},
      [](const auto &acc, const auto &st) { return acc + st.top(); });
}

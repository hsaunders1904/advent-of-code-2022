#include "aoc22/aoc.h"

#include <stack>

namespace {
void fill_level(std::vector<std::vector<char>> *stacks,
                const std::string &line) {
  for (auto i = 0U; i < stacks->size(); ++i) {
    if (line[1 + i * 4] != ' ') {
      (*stacks)[i].emplace_back(line[1 + i * 4]);
    }
  }
}

std::vector<std::stack<char>> read_stacks(std::istream *input_file) {
  std::vector<std::vector<char>> stacks;
  std::string line;
  std::getline(*input_file, line);
  stacks.resize((line.size() + 1) / 4);
  fill_level(&stacks, line);
  for (; std::getline(*input_file, line);) {
    if (line[1] == '1') {
      // Skip empty line
      std::getline(*input_file, line);
      break;
    } else {
      fill_level(&stacks, line);
    }
  }
  std::vector<std::stack<char>> ret_stacks;
  ret_stacks.resize(stacks.size());
  for (auto i = 0U; i < stacks.size(); ++i) {
    for (auto it = stacks[i].rbegin(); it != stacks[i].rend(); ++it) {
      ret_stacks[i].push(*it);
    }
  }
  return ret_stacks;
}

std::vector<std::string> split(const std::string &str, const char split_on) {
  std::vector<std::string> parts;
  std::stringstream ss(str);
  for (std::string part; std::getline(ss, part, split_on);) {
    parts.emplace_back(part);
  }
  return parts;
}

std::array<int, 3> parse_instruction(const std::string &line) {
  std::array<int, 3> instruction;
  auto parts = split(line, ' ');
  instruction[0] = std::stoi(parts[1]);
  instruction[1] = std::stoi(parts[3]) - 1;
  instruction[2] = std::stoi(parts[5]) - 1;
  return instruction;
}

} // namespace

std::string day05_1(std::istream *input_file) {
  auto stacks = read_stacks(input_file);
  for (std::string line; std::getline(*input_file, line);) {
    if (line.empty()) {
      break;
    }
    auto [num_to_move, from, to] = parse_instruction(line);
    for (auto i = 0U; i < num_to_move; ++i) {
      stacks[to].push(stacks[from].top());
      stacks[from].pop();
    }
  }

  std::string out;
  for (const auto &stack : stacks) {
    out += stack.top();
  }
  return out;
}

std::string day05_2(std::istream *input_file) {
  auto stacks = read_stacks(input_file);
  for (std::string line; std::getline(*input_file, line);) {
    if (line.empty()) {
      break;
    }
    auto [num_to_move, from, to] = parse_instruction(line);
    std::vector<char> crates(num_to_move);
    for (auto j = 0U; j < crates.size(); ++j) {
      crates[j] = stacks[from].top();
      stacks[from].pop();
    }
    for (auto it = crates.rbegin(); it != crates.rend(); ++it) {
      stacks[to].push(*it);
    }
  }

  std::string out;
  for (const auto &stack : stacks) {
    out += stack.top();
  }
  return out;
}

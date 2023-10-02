#include "aoc22/utils.h"

#include <iostream>
#include <istream>
#include <string>
#include <unordered_map>

namespace {
std::unordered_map<std::string, std::string> read_monkeys(std::istream *input_file) {
  std::unordered_map<std::string, std::string> monkeys;
  for (std::string line; std::getline(*input_file, line);) {
    auto parts = split(line, ':');
    trim(&parts.at(1));
    monkeys[parts.at(0)] = parts.at(1);
  }
  return monkeys;
}

std::function<long(long, long)> monkey_operation(const char c) {
  switch (c) {
  case '+':
    return std::plus<long>();
  case '-':
    return std::minus<long>();
  case '*':
    return std::multiplies<long>();
  case '/':
    return std::divides<long>();
  default:
    throw std::runtime_error("invalid monkey operation");
  }
}

long monkey_call_number(const std::unordered_map<std::string, std::string> &monkeys,
                        const std::string &monkey) {
  auto operation = monkeys.at(monkey);
  if (operation.find(' ') == std::string::npos) {
    return std::stoi(operation);
  }
  auto parts = split(operation, ' ');
  auto monkey_1_call_number = monkey_call_number(monkeys, parts.at(0));
  auto monkey_2_call_number = monkey_call_number(monkeys, parts.at(2));
  auto monkey_op = monkey_operation(parts.at(1).at(0));
  return monkey_op(monkey_1_call_number, monkey_2_call_number);
}

} // namespace

long day21_1(std::istream *input_file) {
  const auto monkeys = read_monkeys(input_file);
  return monkey_call_number(monkeys, "root");
}

long day21_2(std::istream *input_file) { return 0; }

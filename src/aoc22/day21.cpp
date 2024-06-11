#include "aoc22/utils.h"

#include <cmath>
#include <complex>
#include <functional>
#include <iostream>
#include <istream>
#include <optional>
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

template <typename T> std::function<T(T, T)> monkey_operation(const char c) {
  switch (c) {
  case '+':
    return std::plus<T>();
  case '-':
    return std::minus<T>();
  case '*':
    return std::multiplies<T>();
  case '/':
    return std::divides<T>();
  default:
    throw std::runtime_error("invalid monkey operation");
  }
}

template <typename T>
T monkey_call_number(
    const std::unordered_map<std::string, std::string> &monkeys,
    const std::string &monkey,
    const std::optional<std::pair<std::string, T>> &fix = std::nullopt) {
  if (fix.has_value() && monkey == fix.value().first) {
    return fix.value().second;
  }
  auto operation = monkeys.at(monkey);
  if (operation.find(' ') == std::string::npos) {
    return static_cast<T>(std::stol(operation));
  }
  auto parts = split(operation, ' ');
  auto monkey_1_call_number = monkey_call_number<T>(monkeys, parts.at(0), fix);
  auto monkey_2_call_number = monkey_call_number<T>(monkeys, parts.at(2), fix);
  auto monkey_op = monkey_operation<T>(parts.at(1).at(0));
  return monkey_op(monkey_1_call_number, monkey_2_call_number);
}
} // namespace

long day21_1(std::istream *input_file) {
  const auto monkeys = read_monkeys(input_file);
  return monkey_call_number<long>(monkeys, "root");
}

long day21_2(std::istream *input_file) {
  using namespace std::complex_literals;

  const auto monkeys = read_monkeys(input_file);
  // For this, we leverage complex numbers and set 'humn' to 1i. This way, we can
  // treat the imaginary component of the numbers as the factor or 'humn'. This only
  // works if we never get 'humn * humn', but that seems to be the case!
  constexpr auto fix = std::make_pair("humn", 1.0i);
  auto root_depends = split(monkeys.at("root"), ' ');
  auto lhs = monkey_call_number<std::complex<double>>(monkeys, root_depends.at(0), fix);
  auto rhs = monkey_call_number<std::complex<double>>(monkeys, root_depends.at(2), fix);
  return std::round((lhs.real() - rhs.real()) / (rhs.imag() - lhs.imag()));
}

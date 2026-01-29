#include "aoc22/aoc.h"

#include <algorithm>
#include <string>

namespace {

long snafu_digit_to_int(const char digit) {
  switch (digit) {
  case '2':
    return 2;
  case '1':
    return 1;
  case '0':
    return 0;
  case '-':
    return -1;
  case '=':
    return -2;
  default:
    throw std::runtime_error("invalid snafu digit");
  }
}

long snafu_to_int(const std::string &snafu) {
  long value{0};
  for (const auto &ch : snafu) {
    value = 5 * value + snafu_digit_to_int(ch);
  }
  return value;
}

std::string int_to_snafu(long number) {
  std::string digits;
  while (number > 0) {
    char ch;
    switch (number % 5) {
    case 0: {
      ch = '0';
      break;
    }
    case 1: {
      ch = '1';
      break;
    }
    case 2: {
      ch = '2';
      break;
    }
    case 3: {
      ch = '=';
      break;
    }
    case 4: {
      ch = '-';
      break;
    }
    }
    digits.push_back(ch);
    number = (number + 2) / 5;
  }
  std::reverse(digits.begin(), digits.end());
  return digits;
}

} // namespace

std::string day25_1(std::istream *input_file) {
  long sum{0};
  for (std::string line; std::getline(*input_file, line);) {
    sum += snafu_to_int(line);
  }
  return int_to_snafu(sum);
}

int day25_2(std::istream *) { return 0; }

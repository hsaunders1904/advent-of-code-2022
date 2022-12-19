#include "aoc22/aoc.h"

#include <regex>

namespace {
constexpr char BRACE = '!';

std::string regex_replace(const std::string &str, const std::string &pattern,
                          const std::string &new_str) {
  std::regex re(pattern);
  return std::regex_replace(str, re, new_str);
}

std::string build_comp_str(const std::string &packet) {
  std::string comp_str;
  int brace_level{0};
  for (const auto c : packet) {
    if (c == '[') {
      ++brace_level;
    } else if (c == ']') {
      --brace_level;
    } else if (c == ',') {
      comp_str += BRACE + brace_level;
    } else {
      comp_str += c;
    }
  }
  return regex_replace(comp_str, "10", "A");
}
} // namespace

int day13_1(std::istream *input_file) {
  std::size_t in_order_sum{0};

  int packet_num{1};
  while (!input_file->eof()) {
    std::string left;
    std::string right;
    std::string blank_line;
    std::getline(*input_file, left);
    std::getline(*input_file, right);
    std::getline(*input_file, blank_line);

    auto comp_str_left = build_comp_str(left);
    auto comp_str_right = build_comp_str(right);
    if (comp_str_left < comp_str_right) {
      in_order_sum += packet_num;
    }

    ++packet_num;
  }
  return in_order_sum;
}

int day13_2(std::istream *input_file) { return 1; }

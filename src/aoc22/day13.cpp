#include "aoc22/aoc.h"

#include <regex>

namespace {
constexpr char BRACE = '!';
constexpr auto DIV_2 = "[[2]]";
constexpr auto DIV_6 = "[[6]]";

std::pair<std::string, std::string> read_packet_pair(std::istream *input) {
  std::string left;
  std::string right;
  std::string blank_line;
  std::getline(*input, left);
  std::getline(*input, right);
  std::getline(*input, blank_line);
  return {left, right};
}

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
    const auto [left, right] = read_packet_pair(input_file);
    auto comp_str_left = build_comp_str(left);
    auto comp_str_right = build_comp_str(right);
    if (comp_str_left < comp_str_right) {
      in_order_sum += packet_num;
    }

    ++packet_num;
  }
  return in_order_sum;
}

int day13_2(std::istream *input_file) {
  std::size_t div2_idx{1};
  std::size_t div6_idx{2};
  const auto div_2 = build_comp_str(DIV_2);
  const auto div_6 = build_comp_str(DIV_6);

  std::string packet;
  while (!input_file->eof()) {
    std::getline(*input_file, packet);
    if (packet.empty()) {
      continue;
    }
    const auto comp_str = build_comp_str(packet);
    if (comp_str < div_2) {
      ++div2_idx;
      ++div6_idx;
    } else if (comp_str > div_2 && comp_str < div_6) {
      ++div6_idx;
    }
  }
  return div2_idx * div6_idx;
}

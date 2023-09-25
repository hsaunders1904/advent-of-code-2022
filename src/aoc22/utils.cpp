#include "aoc22/utils.h"

#include <argparse/argparse.hpp>

#include <sstream>

Vec2d<std::size_t>
floyd_warshall(const std::vector<std::vector<std::size_t>> &adj_list) {
  Vec2d<std::size_t> dist(adj_list.size(), adj_list.size(),
                          std::numeric_limits<std::size_t>::max() / 4);
  for (auto i = 0U; i < adj_list.size(); ++i) {
    dist.get(i, i) = 0;
    for (auto neighbour : adj_list[i]) {
      dist.get(i, neighbour) = 1;
    }
  }
  for (auto k = 0U; k < adj_list.size(); ++k) {
    for (auto i = 0U; i < adj_list.size(); ++i) {
      for (auto j = 0U; j < adj_list.size(); ++j) {
        dist.get(i, j) = std::min(dist.get(i, j), dist.get(i, k) + dist.get(k, j));
      }
    }
  }
  return dist;
}

std::vector<std::string> split(const std::string &str, const char split_on) {
  std::vector<std::string> parts;
  std::stringstream ss(str);
  for (std::string part; std::getline(ss, part, split_on);) {
    parts.emplace_back(part);
  }
  return parts;
}

std::vector<std::string> split(const std::string &str, const std::string &split_on) {
  std::vector<std::string> parts;
  auto beg = str.begin();
  auto split_pos = str.begin();
  while (beg <= str.end()) {
    split_pos = std::find_first_of(beg, str.end(), split_on.begin(), split_on.end());
    parts.emplace_back(beg, split_pos);
    if (split_pos == str.end()) {
      break;
    }
    beg = std::next(split_pos, split_on.size());
  }
  return parts;
}

void ltrim(std::string *str) {
  str->erase(str->begin(), std::find_if(str->begin(), str->end(), [](unsigned char ch) {
               return std::isalnum(ch);
             }));
}

void rtrim(std::string *str) {
  str->erase(std::find_if(str->rbegin(), str->rend(),
                          [](unsigned char ch) { return std::isalnum(ch); })
                 .base(),
             str->end());
}

void trim(std::string *str) {
  ltrim(str);
  rtrim(str);
}

namespace details {
std::ifstream open_file(const std::string &path) {
  std::ifstream stream(path);
  if (!stream.is_open()) {
    throw std::runtime_error("could not open file '" + path + "'.");
  }
  return stream;
}

Opts parse_args(int argc, char *argv[]) {
  argparse::ArgumentParser parser("aoc22");
  parser.add_argument("data_file");
  parser.add_argument("-p1", "--part1")
      .implicit_value(true)
      .default_value(false)
      .help("run part 1");
  parser.add_argument("-p2", "--part2")
      .implicit_value(true)
      .default_value(false)
      .help("run part 2");
  try {
    parser.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    std::exit(1);
  }
  std::string data_file = parser.get("data_file");
  auto part_1 = parser.get<bool>("part1");
  auto part_2 = parser.get<bool>("part2");
  return {data_file, part_1 || !part_2, part_2 || !part_1};
}
} // namespace details

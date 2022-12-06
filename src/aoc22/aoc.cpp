#include "aoc22/aoc.h"

#include <argparse/argparse.hpp>

std::vector<std::string> split(const std::string &str, const char split_on) {
  std::vector<std::string> parts;
  std::stringstream ss(str);
  for (std::string part; std::getline(ss, part, split_on);) {
    parts.emplace_back(part);
  }
  return parts;
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

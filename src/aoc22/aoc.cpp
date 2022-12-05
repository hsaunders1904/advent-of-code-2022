#include "aoc22/aoc.h"

std::ifstream open_file(const std::string &path) {
  std::ifstream stream(path);
  if (!stream.is_open()) {
    throw std::runtime_error("could not open file '" + path + "'.");
  }
  return stream;
}

std::vector<std::string> split(const std::string &str, const char split_on) {
  std::vector<std::string> parts;
  std::stringstream ss(str);
  for (std::string part; std::getline(ss, part, split_on);) {
    parts.emplace_back(part);
  }
  return parts;
}

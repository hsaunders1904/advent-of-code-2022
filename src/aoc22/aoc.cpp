#include "aoc22/aoc.h"

std::ifstream open_file(const std::string &path) {
  std::ifstream stream(path);
  if (!stream.is_open()) {
    throw std::runtime_error("could not open file '" + path + "'.");
  }
  return stream;
}

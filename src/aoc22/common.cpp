#include "aoc22/aoc.h"

#include <string>
#include <vector>

std::ifstream open_file(const std::string &path) {
  std::ifstream stream(path);
  if (!stream.is_open()) {
    throw std::runtime_error("could not open file '" + path + "'.");
  }
  return stream;
}

std::string parse_one_cli_arg(int argc, char *argv[]) {
  std::vector<std::string> args(argv, argv + argc);
  if (args.size() != 2) {
    throw std::runtime_error(
        "must provide exactly one argument on command line. Found " +
        std::to_string(args.size() - 1) + ".");
  }
  return args[1];
}

#include "aoc22/aoc.h"

#include <iostream>
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

int run_day(int argc, char *argv[], day_func part_1, day_func part_2) {
  auto input_file = parse_one_cli_arg(argc, argv);
  auto stream = open_file(input_file);
  std::cout << part_1(&stream) << std::endl;
  stream.clear();
  stream.seekg(0, std::ios::beg);
  std::cout << part_2(&stream) << std::endl;
  return 0;
}

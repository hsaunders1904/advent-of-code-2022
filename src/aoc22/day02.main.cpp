#include "aoc22/aoc.h"

#include <iostream>

int main(int argc, char *argv[]) {
  auto input_file = parse_one_cli_arg(argc, argv);
  auto stream = open_file(input_file);
  std::cout << day02_1(&stream) << std::endl;
  stream.clear();
  stream.seekg(0, std::ios::beg);
  std::cout << day02_2(&stream) << std::endl;
  return 0;
}

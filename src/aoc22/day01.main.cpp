#include "aoc22/aoc.h"

#include <iostream>

int main(int argc, char *argv[]) {
  auto input_file = parse_one_cli_arg(argc, argv);
  auto stream = open_file(input_file);
  std::cout << day01(&stream) << std::endl;
  return 0;
}

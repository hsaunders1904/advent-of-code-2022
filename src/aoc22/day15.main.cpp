#include "aoc22/aoc.h"

int main(int argc, char *argv[]) {
  auto part_1 = [](std::istream *is) { return day15_1(is, 2000000); };
  run_day<int, int>(argc, argv, part_1, day15_2);
  return 0;
}

#include "aoc22/aoc.h"

int main(int argc, char *argv[]) {
  auto part_1 = [](std::istream *is) { return day15_1(is, 2'000'000); };
  auto part_2 = [](std::istream *is) { return day15_2(is, 4'000'000); };
  run_day<int, std::size_t>(argc, argv, part_1, part_2);
  return 0;
}

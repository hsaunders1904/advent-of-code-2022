#include "aoc22/aoc.h"

#include <gtest/gtest.h>

#include <sstream>

namespace {
constexpr auto day01_data =
    "1000\n\n2000\n3000\n\n4000\n\n5000\n6000\n\n7000\n8000\n9000\n\n"
    "10000\n\n";
constexpr auto day02_data = "A Y\nB X\nC Z\n";
constexpr auto day03_data = "vJrwpWtwJgWrhcsFMMfFFhFp\n"
                            "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\n"
                            "PmmdzqPrVvPwwTWBwg\n"
                            "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\n"
                            "ttgJtRGJQctTZtZT\n"
                            "CrZsJsPPZsGzwwsLwLmpwMDw\n";
} // namespace

TEST(day01, Part1) {
  std::stringstream stream(day01_data);

  EXPECT_EQ(day01_1(&stream), 24000);
}

TEST(day01, Part2) {
  std::stringstream stream(day01_data);

  EXPECT_EQ(day01_2(&stream), 45000);
}

TEST(day02, Part1) {
  std::stringstream stream(day02_data);

  EXPECT_EQ(day02_1(&stream), 15);
}

TEST(day02, Part2) {
  std::stringstream stream(day02_data);

  EXPECT_EQ(day02_2(&stream), 12);
}

TEST(day03, Part1) {
  std::stringstream stream(day03_data);

  EXPECT_EQ(day03_1(&stream), 157);
}

#include "aoc22/aoc.h"

#include <gtest/gtest.h>

#include <sstream>

namespace data {
constexpr auto day01 =
    "1000\n\n2000\n3000\n\n4000\n\n5000\n6000\n\n7000\n8000\n9000\n\n"
    "10000\n\n";
constexpr auto day02 = "A Y\nB X\nC Z\n";
constexpr auto day03 = "vJrwpWtwJgWrhcsFMMfFFhFp\n"
                       "jqHRNqRjqzjGDLGLrsFMfFZSrLrFZsSL\n"
                       "PmmdzqPrVvPwwTWBwg\n"
                       "wMqvLMZHhHMvwLHjbvcjnnSBnvTQFn\n"
                       "ttgJtRGJQctTZtZT\n"
                       "CrZsJsPPZsGzwwsLwLmpwMDw\n";
constexpr auto day04 = "2-4,6-8\n"
                       "2-3,4-5\n"
                       "5-7,7-9\n"
                       "2-8,3-7\n"
                       "6-6,4-6\n"
                       "2-6,4-8\n";
constexpr auto day05 = "    [D]    \n"
                       "[N] [C]    \n"
                       "[Z] [M] [P]\n"
                       " 1   2   3 \n"
                       "\n"
                       "move 1 from 2 to 1\n"
                       "move 3 from 1 to 3\n"
                       "move 2 from 2 to 1\n"
                       "move 1 from 1 to 2\n";
} // namespace data

TEST(day01, Part1) {
  std::stringstream stream(data::day01);

  EXPECT_EQ(day01_1(&stream), 24000);
}

TEST(day01, Part2) {
  std::stringstream stream(data::day01);

  EXPECT_EQ(day01_2(&stream), 45000);
}

TEST(day02, Part1) {
  std::stringstream stream(data::day02);

  EXPECT_EQ(day02_1(&stream), 15);
}

TEST(day02, Part2) {
  std::stringstream stream(data::day02);

  EXPECT_EQ(day02_2(&stream), 12);
}

TEST(day03, Part1) {
  std::stringstream stream(data::day03);

  EXPECT_EQ(day03_1(&stream), 157);
}

TEST(day03, Part2) {
  std::stringstream stream(data::day03);

  EXPECT_EQ(day03_2(&stream), 70);
}

TEST(day04, Part1) {
  std::stringstream stream(data::day04);

  EXPECT_EQ(day04_1(&stream), 2);
}

TEST(day04, Part2) {
  std::stringstream stream(data::day04);

  EXPECT_EQ(day04_2(&stream), 4);
}

TEST(day05, Part1) {
  std::stringstream stream(data::day05);

  EXPECT_EQ(day05_1(&stream), "CMZ");
}

TEST(day05, Part2) {
  std::stringstream stream(data::day05);

  EXPECT_EQ(day05_2(&stream), "MCD");
}

TEST(day06, Part1_1) {
  std::stringstream stream("mjqjpqmgbljsphdztnvjfqwrcgsmlb");

  EXPECT_EQ(day06_1(&stream), 7);
}

TEST(day06, Part1_2) {
  std::stringstream stream("bvwbjplbgvbhsrlpgdmjqwftvncz");

  EXPECT_EQ(day06_1(&stream), 5);
}

TEST(day06, Part1_3) {
  std::stringstream stream("nppdvjthqldpwncqszvftbrmjlhg");

  EXPECT_EQ(day06_1(&stream), 6);
}

TEST(day06, Part1_4) {
  std::stringstream stream("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg");

  EXPECT_EQ(day06_1(&stream), 10);
}

TEST(day06, Part1_5) {
  std::stringstream stream("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw");

  EXPECT_EQ(day06_1(&stream), 11);
}

TEST(day06, Part2_1) {
  std::stringstream stream("mjqjpqmgbljsphdztnvjfqwrcgsmlb");

  EXPECT_EQ(day06_2(&stream), 19);
}

TEST(day06, Part2_2) {
  std::stringstream stream("bvwbjplbgvbhsrlpgdmjqwftvncz");

  EXPECT_EQ(day06_2(&stream), 23);
}

TEST(day06, Part2_3) {
  std::stringstream stream("nppdvjthqldpwncqszvftbrmjlhg");

  EXPECT_EQ(day06_2(&stream), 23);
}

TEST(day06, Part2_4) {
  std::stringstream stream("nznrnfrfntjfmvfwmzdfjlvtqnbhcprsg");

  EXPECT_EQ(day06_2(&stream), 29);
}

TEST(day06, Part2_5) {
  std::stringstream stream("zcfzfwzzqfrljwzlrfnpqdbhtmscgvjw");

  EXPECT_EQ(day06_2(&stream), 26);
}

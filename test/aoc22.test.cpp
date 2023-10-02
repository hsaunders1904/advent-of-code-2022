#include "./test_data.h"
#include "aoc22/aoc.h"

#include <gtest/gtest.h>

#include <sstream>

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

TEST(day07, Part1) {
  std::stringstream stream(data::day07);

  EXPECT_EQ(day07_1(&stream), 95437);
}

TEST(day07, Part2) {
  std::stringstream stream(data::day07);

  EXPECT_EQ(day07_2(&stream), 24933642);
}

TEST(day08, Part1) {
  std::stringstream stream(data::day08);

  EXPECT_EQ(day08_1(&stream), 21);
}

TEST(day08, Part2) {
  std::stringstream stream(data::day08);

  EXPECT_EQ(day08_2(&stream), 8);
}

TEST(day09, Part1) {
  constexpr auto instructions = "R 4\n"
                                "U 4\n"
                                "L 3\n"
                                "D 1\n"
                                "R 4\n"
                                "D 1\n"
                                "L 5\n"
                                "R 2\n";
  std::stringstream stream(instructions);

  EXPECT_EQ(day09_1(&stream), 13);
}

TEST(day09, Part2) {
  constexpr auto instructions = "R 5\n"
                                "U 8\n"
                                "L 8\n"
                                "D 3\n"
                                "R 17\n"
                                "D 10\n"
                                "L 25\n"
                                "U 20\n";
  std::stringstream stream(instructions);

  EXPECT_EQ(day09_2(&stream), 36);
}

TEST(day10, Part1) {
  std::stringstream stream(data::day10);

  EXPECT_EQ(day10_1(&stream), 13140);
}

TEST(day10, Part2) {
  std::stringstream stream(data::day10);
  std::string expected = "##..##..##..##..##..##..##..##..##..##..\n"
                         "###...###...###...###...###...###...###.\n"
                         "####....####....####....####....####....\n"
                         "#####.....#####.....#####.....#####.....\n"
                         "######......######......######......####\n"
                         "#######.......#######.......#######.....\n";

  EXPECT_EQ(day10_2(&stream), expected);
}

TEST(day11, Part1) {
  std::stringstream stream(data::day11);

  EXPECT_EQ(day11_1(&stream), 10605);
}

TEST(day11, Part2) {
  std::stringstream stream(data::day11);

  EXPECT_EQ(day11_2(&stream), 2713310158);
}

TEST(day12, Part1) {
  std::stringstream stream(data::day12);

  EXPECT_EQ(day12_1(&stream), 31);
}

TEST(day12, Part2) {
  std::stringstream stream(data::day12);

  EXPECT_EQ(day12_2(&stream), 29);
}

TEST(day13, Part1) {
  std::stringstream stream(data::day13);

  EXPECT_EQ(day13_1(&stream), 13);
}

TEST(day13, Part2) {
  std::stringstream stream(data::day13);

  EXPECT_EQ(day13_2(&stream), 140);
}

TEST(day14, Part1) {
  std::stringstream stream(data::day14);

  EXPECT_EQ(day14_1(&stream), 24);
}

TEST(day14, Part2) {
  std::stringstream stream(data::day14);

  EXPECT_EQ(day14_2(&stream), 93);
}

TEST(day15, Part1) {
  std::stringstream stream(data::day15);

  EXPECT_EQ(day15_1(&stream, 10), 26);
}

TEST(day15, Part2) {
  std::stringstream stream(data::day15);

  EXPECT_EQ(day15_2(&stream, 20), 56000011);
}

TEST(day16, Part1) {
  std::stringstream stream(data::day16);

  EXPECT_EQ(day16_1(&stream), 1651);
}

TEST(day16, Part2) {
  std::stringstream stream(data::day16);

  EXPECT_EQ(day16_2(&stream), 1707);
}

TEST(day17, Part1) {
  std::stringstream stream(data::day17);

  EXPECT_EQ(day17_1(&stream), 3068);
}

TEST(day17, Part2) {
  std::stringstream stream(data::day17);

  EXPECT_EQ(day17_2(&stream), 1514285714288);
}

TEST(day18, Part1) {
  std::stringstream stream(data::day18);

  EXPECT_EQ(day18_1(&stream), 64);
}

TEST(day18, Part2) {
  std::stringstream stream(data::day18);

  EXPECT_EQ(day18_2(&stream), 58);
}

TEST(day19, Part1) {
  std::stringstream stream(data::day19);

  EXPECT_EQ(day19_1(&stream), 33);
}

TEST(day19, Part2) {
  std::stringstream stream(data::day19);

  EXPECT_EQ(day19_2(&stream), 62 * 56);
}

TEST(day20, Part1) {
  std::stringstream stream(data::day20);

  EXPECT_EQ(day20_1(&stream), 3);
}

TEST(day20, Part2) {
  std::stringstream stream(data::day20);

  EXPECT_EQ(day20_2(&stream), 1623178306);
}

TEST(day21, Part1) {
  std::stringstream stream(data::day21);

  EXPECT_EQ(day21_1(&stream), 152);
}

TEST(day21, Part2) {
  std::stringstream stream(data::day21);

  EXPECT_EQ(day21_2(&stream), 301);
}

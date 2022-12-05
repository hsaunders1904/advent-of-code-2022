#pragma once

#include <argparse/argparse.hpp>

#include <fstream>
#include <iostream>
#include <string>

template <typename T> using day_function = std::function<T(std::istream *)>;

namespace {
struct Opts {
  std::string data_file;
  bool part_1;
  bool part_2;
};

inline Opts parse_args(int argc, char *argv[]) {
  argparse::ArgumentParser parser("aoc22");
  parser.add_argument("data_file");
  parser.add_argument("-p1", "--part1")
      .implicit_value(true)
      .default_value(false)
      .help("run part 1");
  parser.add_argument("-p2", "--part2")
      .implicit_value(true)
      .default_value(false)
      .help("run part 2");
  try {
    parser.parse_args(argc, argv);
  } catch (const std::runtime_error &err) {
    std::cerr << err.what() << std::endl;
    std::cerr << parser;
    std::exit(1);
  }
  std::string data_file = parser.get("data_file");
  auto part_1 = parser.get<bool>("part1");
  auto part_2 = parser.get<bool>("part2");
  return {data_file, part_1 || !part_2, part_2 || !part_1};
}
} // namespace

std::ifstream open_file(const std::string &path);
template <typename T>
int run_day(int argc, char *argv[], day_function<T> part_1,
            day_function<T> part_2) {
  auto args = parse_args(argc, argv);
  auto stream = open_file(args.data_file);
  if (args.part_1) {
    std::cout << part_1(&stream) << std::endl;
    if (args.part_2) {
      stream.clear();
      stream.seekg(0, std::ios::beg);
    }
  }
  if (args.part_2) {
    std::cout << part_2(&stream) << std::endl;
  }
  return 0;
}

int day01_1(std::istream *input_file);
int day01_2(std::istream *input_file);
int day02_1(std::istream *input_file);
int day02_2(std::istream *input_file);
int day03_1(std::istream *input_file);
int day03_2(std::istream *input_file);
int day04_1(std::istream *input_file);
int day04_2(std::istream *input_file);

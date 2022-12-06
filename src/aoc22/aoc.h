#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <typename T> using day_func = std::function<T(std::istream *)>;

struct Opts {
  std::string data_file;
  bool part_1;
  bool part_2;
};

Opts parse_args(int argc, char *argv[]);
std::ifstream open_file(const std::string &path);
std::vector<std::string> split(const std::string &str, const char split_on);
template <typename T>
int run_day(int argc, char *argv[], day_func<T> part_1, day_func<T> part_2);

int day01_1(std::istream *input_file);
int day01_2(std::istream *input_file);
int day02_1(std::istream *input_file);
int day02_2(std::istream *input_file);
int day03_1(std::istream *input_file);
int day03_2(std::istream *input_file);
int day04_1(std::istream *input_file);
int day04_2(std::istream *input_file);
std::string day05_1(std::istream *input_file);
std::string day05_2(std::istream *input_file);
int day06_1(std::istream *input_file);
int day06_2(std::istream *input_file);

template <typename T>
int run_day(int argc, char *argv[], day_func<T> part_1, day_func<T> part_2) {
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

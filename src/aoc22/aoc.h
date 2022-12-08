#pragma once

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

template <typename T> using day_func = std::function<T(std::istream *)>;

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
int day07_1(std::istream *input_file);
int day07_2(std::istream *input_file);
int day08_1(std::istream *input_file);
int day08_2(std::istream *input_file);

std::vector<std::string> split(const std::string &str, const char split_on);

namespace details {
struct Opts {
  std::string data_file;
  bool part_1;
  bool part_2;
};
Opts parse_args(int argc, char *argv[]);
std::ifstream open_file(const std::string &path);
} // namespace details

template <typename T>
int run_day(int argc, char *argv[], day_func<T> part_1, day_func<T> part_2) {
  auto args = details::parse_args(argc, argv);
  auto stream = details::open_file(args.data_file);
  if (args.part_1) {
    auto start = std::chrono::steady_clock::now().time_since_epoch().count();
    auto p1 = part_1(&stream);
    auto end = std::chrono::steady_clock::now().time_since_epoch().count();
    std::cout << p1 << " (in " << std::scientific << (end - start) * 1e-9
              << " s)\n";
    if (args.part_2) {
      stream.clear();
      stream.seekg(0, std::ios::beg);
    }
  }
  if (args.part_2) {
    auto start = std::chrono::steady_clock::now().time_since_epoch().count();
    auto p2 = part_2(&stream);
    auto end = std::chrono::steady_clock::now().time_since_epoch().count();
    std::cout << p2 << " (in " << std::scientific << (end - start) * 1e-9
              << " s)\n";
  }
  return 0;
}

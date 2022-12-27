#pragma once

#include <array>
#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

template <typename T> using day_func = std::function<T(std::istream *)>;
using std::chrono::high_resolution_clock;

std::vector<std::string> split(const std::string &str, const char split_on);
std::vector<std::string> split(const std::string &str, const std::string &split_on);
void ltrim(std::string *str);
void rtrim(std::string *str);
void trim(std::string *str);

template <typename T> class Vec2d {
public:
  Vec2d(std::size_t rows, std::size_t cols, T initial_value)
      : m_size({rows, cols}), m_data(rows * cols, initial_value) {}
  T &get(std::size_t row, std::size_t col) { return m_data[m_size[1] * row + col]; }
  T at(std::size_t row, std::size_t col) const {
    return m_data.at(m_size[1] * row + col);
  }
  std::array<std::size_t, 2> size() const { return m_size; }

  friend void operator<<(std::ostream &os, const Vec2d<T> &vec) {
    for (auto i = 0U; i < vec.size()[0]; ++i) {
      for (auto j = 0U; j < vec.size()[1]; ++j) {
        os << vec.at(i, j) << " ";
      }
      os << "\n";
    }
  }

private:
  std::array<std::size_t, 2> m_size;
  std::vector<T> m_data;
};

namespace details {
struct Opts {
  std::string data_file;
  bool part_1;
  bool part_2;
};
Opts parse_args(int argc, char *argv[]);
std::ifstream open_file(const std::string &path);
} // namespace details

template <typename T, typename U>
int run_day(int argc, char *argv[], day_func<T> part_1, day_func<U> part_2) {
  auto args = details::parse_args(argc, argv);
  auto stream = details::open_file(args.data_file);
  if (args.part_1) {
    auto start = high_resolution_clock::now().time_since_epoch().count();
    auto p1 = part_1(&stream);
    auto end = high_resolution_clock::now().time_since_epoch().count();
    std::cout << p1 << " (in " << std::scientific << (end - start) * 1e-9 << " s)\n";
    if (args.part_2) {
      stream.clear();
      stream.seekg(0, std::ios::beg);
    }
  }
  if (args.part_2) {
    auto start = high_resolution_clock::now().time_since_epoch().count();
    auto p2 = part_2(&stream);
    auto end = high_resolution_clock::now().time_since_epoch().count();
    std::cout << p2 << " (in " << std::scientific << (end - start) * 1e-9 << " s)\n";
  }
  return 0;
}

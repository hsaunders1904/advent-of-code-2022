#include "aoc22/aoc.h"

#include <benchmark/benchmark.h>

#include <sstream>

namespace {
std::string read_text(const std::string &path) {
  std::ifstream ifs(path);
  if (!ifs.is_open()) {
    throw std::runtime_error("cannot open file '" + path + "'");
  }
  return {std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>()};
}
} // namespace

#define AOC_BENCHMARK(bm_name, day_func, data_path)                                     \
  static void bm_name(benchmark::State &state) {                                        \
    auto text_data = read_text(data_path);                                              \
    for (auto _ : state) {                                                              \
      state.PauseTiming();                                                              \
      std::stringstream data(text_data);                                                \
      state.ResumeTiming();                                                             \
      day_func(&data);                                                                  \
    }                                                                                   \
  }                                                                                     \
  BENCHMARK(bm_name)->Unit(benchmark::kMillisecond);

AOC_BENCHMARK(BM_day01_1, day01_1, "../data/day01.txt");
AOC_BENCHMARK(BM_day01_2, day01_2, "../data/day01.txt");
AOC_BENCHMARK(BM_day02_1, day02_1, "../data/day02.txt");
AOC_BENCHMARK(BM_day02_2, day02_2, "../data/day02.txt");
AOC_BENCHMARK(BM_day03_1, day03_1, "../data/day03.txt");
AOC_BENCHMARK(BM_day03_2, day03_2, "../data/day03.txt");
AOC_BENCHMARK(BM_day04_1, day04_1, "../data/day04.txt");
AOC_BENCHMARK(BM_day04_2, day04_2, "../data/day04.txt");
AOC_BENCHMARK(BM_day05_1, day05_1, "../data/day05.txt");
AOC_BENCHMARK(BM_day05_2, day05_2, "../data/day05.txt");
AOC_BENCHMARK(BM_day06_1, day06_1, "../data/day06.txt");
AOC_BENCHMARK(BM_day06_2, day06_2, "../data/day06.txt");
AOC_BENCHMARK(BM_day07_1, day07_1, "../data/day07.txt");
AOC_BENCHMARK(BM_day07_2, day07_2, "../data/day07.txt");
AOC_BENCHMARK(BM_day08_1, day08_1, "../data/day08.txt");
AOC_BENCHMARK(BM_day08_2, day08_2, "../data/day08.txt");
AOC_BENCHMARK(BM_day09_1, day09_1, "../data/day09.txt");
AOC_BENCHMARK(BM_day09_2, day09_2, "../data/day09.txt");
AOC_BENCHMARK(BM_day10_1, day10_1, "../data/day10.txt");
AOC_BENCHMARK(BM_day10_2, day10_2, "../data/day10.txt");
AOC_BENCHMARK(BM_day11_1, day11_1, "../data/day11.txt");
AOC_BENCHMARK(BM_day11_2, day11_2, "../data/day11.txt");
AOC_BENCHMARK(BM_day12_1, day12_1, "../data/day12.txt");
AOC_BENCHMARK(BM_day12_2, day12_2, "../data/day12.txt");
AOC_BENCHMARK(BM_day13_1, day13_1, "../data/day13.txt");
AOC_BENCHMARK(BM_day13_2, day13_2, "../data/day13.txt");
AOC_BENCHMARK(BM_day14_1, day14_1, "../data/day14.txt");
AOC_BENCHMARK(BM_day14_2, day14_2, "../data/day14.txt");
AOC_BENCHMARK(
    BM_day15_1, [](auto *is) { return day15_1(is, 2'000'000); }, "../data/day15.txt");
AOC_BENCHMARK(
    BM_day15_2, [](auto *is) { return day15_2(is, 4'000'000); }, "../data/day15.txt");
AOC_BENCHMARK(BM_day16_1, day16_1, "../data/day16.txt");
AOC_BENCHMARK(BM_day16_2, day16_2, "../data/day16.txt");
AOC_BENCHMARK(BM_day17_1, day17_1, "../data/day17.txt");
AOC_BENCHMARK(BM_day17_2, day17_2, "../data/day17.txt");

BENCHMARK_MAIN();

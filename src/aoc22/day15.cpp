#include "aoc22/aoc.h"

#include <array>
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace {
constexpr auto DATA_LINE_PATTERN =
    ".* x=(-?[0-9]+), y=(-?[0-9]+): .* x=(-?[0-9]+), y=(-?[0-9]+)$";

struct PosHash;

using Pos = std::array<int, 2>;
using PosMap = std::unordered_map<Pos, Pos, PosHash>;

struct PosHash {
  std::size_t operator()(const Pos &pos) const {
    constexpr uint64_t size_of_int{32};
    return static_cast<uint64_t>(static_cast<uint>(pos[0])) << size_of_int |
           static_cast<uint>(pos[1]);
  }
};

PosMap read_positions(std::istream *input) {
  PosMap positions;
  std::regex re(DATA_LINE_PATTERN);
  std::smatch matches;
  for (std::string line; std::getline(*input, line);) {
    std::regex_search(line, matches, re);
    Pos sensor = {std::stoi(matches[1].str()), std::stoi(matches[2].str())};
    Pos beacon = {std::stoi(matches[3].str()), std::stoi(matches[4].str())};
    positions[sensor] = beacon;
  }
  return positions;
}

int manhattan_dist(const Pos &a, const Pos &b) {
  return std::abs(a[0] - b[0]) + std::abs(a[1] - b[1]);
}

std::unordered_set<int> beacons_in_row(const PosMap &positions, const int row) {
  std::unordered_set<int> x_positions;
  for (const auto &[_, beacon] : positions) {
    if (beacon[1] == row) {
      x_positions.insert(beacon[0]);
    }
  }
  return x_positions;
}
} // namespace

int day15_1(std::istream *input_file, const int row_to_check) {
  std::unordered_set<int> row_forbidden;
  const auto positions = read_positions(input_file);
  for (const auto &[sensor, beacon] : positions) {
    const auto dist = manhattan_dist(sensor, beacon);
    const auto y_dist = std::abs(sensor[1] - row_to_check);
    const auto num_in_row = dist - y_dist + 1;
    if (num_in_row <= 0) {
      // Sensor is not close enough to row to have any effect
      continue;
    }
    row_forbidden.insert(sensor[0]);
    for (auto i = 1; i < num_in_row; ++i) {
      row_forbidden.insert(sensor[0] + i);
      row_forbidden.insert(sensor[0] - i);
    }
  }
  for (auto b : beacons_in_row(positions, row_to_check)) {
    row_forbidden.erase(b);
  }
  return row_forbidden.size();
}

int day15_2(std::istream *input_file) { return 1; }

#include <array>
#include <istream>
#include <regex>
#include <unordered_map>
#include <unordered_set>

namespace {
constexpr auto DATA_LINE_PATTERN =
    ".* x=(-?[0-9]+), y=(-?[0-9]+): .* x=(-?[0-9]+), y=(-?[0-9]+)$";
constexpr std::size_t TUNING_FACTOR{4'000'000};

struct PosHash;

using Pos = std::array<int, 2>;
using PosMap = std::unordered_map<Pos, Pos, PosHash>;

struct PosHash {
  std::size_t operator()(const Pos &pos) const {
    // No need for a proper hash here. Just use the first 32 bits to store x, and the
    // final 32 to store y.
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

struct Region {
  std::array<int, 2> x_lims;
  std::array<int, 2> y_lims;
  std::array<Pos, 4> corners() const {
    Pos c1({x_lims[0], y_lims[0]});
    Pos c2({x_lims[0], y_lims[1]});
    Pos c3({x_lims[1], y_lims[0]});
    Pos c4({x_lims[1], y_lims[1]});
    return {c1, c2, c3, c4};
  }
  bool size_is_1() const { return x_lims[0] == x_lims[1] && y_lims[0] == y_lims[1]; }
};

std::array<Region, 4> quarter(const Region &region) {
  auto min_x = region.x_lims[0];
  auto max_x = region.x_lims[1];
  auto min_y = region.y_lims[0];
  auto max_y = region.y_lims[1];
  auto mid_x = (max_x + min_x) / 2;
  auto mid_y = (max_y + min_y) / 2;

  Region lower_left({{min_x, mid_x}, {min_y, mid_y}});
  Region upper_left({{min_x, mid_x}, {std::min(mid_y + 1, max_y), max_y}});
  Region lower_right({{std::min(mid_x + 1, max_x), max_x}, {min_y, mid_y}});
  Region upper_right(
      {{std::min(mid_x + 1, max_x), max_x}, {std::min(mid_y + 1, max_y), max_y}});
  return {lower_left, upper_left, lower_right, upper_right};
}

bool contains_uncovered(const Region &region, const std::pair<Pos, Pos> &sensor_beacon) {
  // If the distance to each corner is less than the sensor's radius, then there are no
  // uncovered points in the region. Note, we use the converse here; if the max corner
  // distance is greater than the radius, there are some uncovered points in the region.
  auto max_dist = 0;
  for (const auto &corner : region.corners()) {
    auto dist_to_corner = manhattan_dist(corner, sensor_beacon.first);
    max_dist = std::max(dist_to_corner, max_dist);
  }
  const auto radius = manhattan_dist(sensor_beacon.first, sensor_beacon.second);
  return max_dist > radius;
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

std::size_t day15_2(std::istream *input_file, const int max_coord) {
  const auto positions = read_positions(input_file);
  // Break the region into quadrants, check if the beacon can exist in each quadrant,
  // then, if it can, break that quadrant down. Repeat until left with a quadrant
  // consisting of a single point.
  Region full_region({{0, max_coord}, {0, max_coord}});
  std::vector<Region> regions = {full_region};
  while (regions.size() > 0) {
    auto region = regions[regions.size() - 1];
    regions.pop_back();
    if (region.size_is_1()) {
      return static_cast<std::size_t>(region.x_lims[0]) * TUNING_FACTOR +
             static_cast<std::size_t>(region.y_lims[0]);
    }
    for (const auto &quadrant : quarter(region)) {
      if (std::all_of(positions.begin(), positions.end(), [&quadrant](const auto &sb) {
            return contains_uncovered(quadrant, sb);
          })) {
        regions.emplace_back(quadrant);
      }
    }
  }
  return 0;
}

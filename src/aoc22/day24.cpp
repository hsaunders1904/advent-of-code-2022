#include "aoc22/aoc.h"

#include <bitset>
#include <cassert>
#include <cstddef>
#include <istream>

namespace {
constexpr std::size_t MAX_WIDTH{192};
using Slice = std::bitset<MAX_WIDTH>;
enum class TraversalDirection { TopToBottom, BottomToTop };

struct BlizzardMap {
  static BlizzardMap from_stream(std::istream *input);
  std::size_t width;
  std::size_t height;
  /// Vector of precomputed horizontal blizzard positions, one for each time step.
  /// The blizzards are cyclic, so will repeat after `width`/`height` time steps.
  /// We can save ourselves some bother by precomputing all the possible positions.
  std::vector<Slice> horizontal;
  /// Vector of precomputed vertical blizzard positions, one for each time step.
  std::vector<Slice> vertical;
};

std::vector<Slice> row_from_string(const std::vector<std::string> &raw, char kind,
                                   std::size_t width, std::size_t height) {
  std::vector<Slice> result(height);
  for (auto y = 0U; y < height; ++y) {
    Slice row;
    for (auto x = 0U; x < width; ++x) {
      // 1 = free, 0 = blizzard
      row[width - 1 - x] = (raw[y + 1][x] != kind);
    }
    result[y] = row;
  }
  return result;
}

Slice rotl(const Slice &r, int k, int w) { return (r << k) | (r >> (w - k)); }

Slice rotr(const Slice &r, int k, int w) { return (r >> k) | (r << (w - k)); }

BlizzardMap BlizzardMap::from_stream(std::istream *is) {
  std::vector<std::string> lines;
  for (std::string line; std::getline(*is, line);) {
    lines.push_back(line.substr(1, line.size() - 2)); // strip walls
  }

  std::size_t width = lines[0].size();
  std::size_t height = lines.size() - 2;
  assert(width <= MAX_WIDTH && height <= MAX_WIDTH);

  BlizzardMap map;
  map.width = width;
  map.height = height;

  // Precompute horizontal blizzard positions.
  auto left = row_from_string(lines, '<', width, height);
  auto right = row_from_string(lines, '>', width, height);
  map.horizontal.reserve(width * height);
  for (auto t = 0U; t < width; ++t) {
    for (auto y = 0U; y < height; ++y) {
      Slice l = rotl(left[y], t, width);
      Slice r = rotr(right[y], t, width);
      map.horizontal.push_back(l & r);
    }
  }
  // And the vertical blizzard positions.
  auto up = row_from_string(lines, '^', width, height);
  auto down = row_from_string(lines, 'v', width, height);
  map.vertical.reserve(height * height);
  for (auto t = 0U; t < height; ++t) {
    for (auto y = 0U; y < height; ++y) {
      Slice u = up[(y + t) % height];
      Slice d = down[(height + y - (t % height)) % height];
      map.vertical.push_back(u & d);
    }
  }

  return map;
}

template <TraversalDirection direction>
int simulate(const BlizzardMap &map, int start_time = 0) {
  int time = start_time;
  // The x positions reachable at row y.
  std::vector<Slice> state(map.height + 1);

  while (true) {
    ++time;

    Slice prev, cur, next;
    next = state[0];

    for (auto y = 0U; y < map.height; ++y) {
      prev = cur;
      cur = next;
      next = state[y + 1];

      Slice spread = cur          // wait
                     | (cur << 1) // move left
                     | (cur >> 1) // move right
                     | prev       // move up
                     | next;      // move down
      // Mask out blizzard positions.
      spread &= map.horizontal[map.height * (time % map.width) + y];
      spread &= map.vertical[map.height * (time % map.height) + y];
      state[y] = spread;
    }

    if constexpr (direction == TraversalDirection::BottomToTop) {
      state[0].set(map.width - 1);
      // End point is always at the bottom right.
      if (state[map.height - 1].test(0)) {
        return time + 1;
      }
    } else {
      state[map.height - 1].set(0);
      // Start point is always at the top left.
      if (state[0].test(map.width - 1)) {
        return time + 1;
      }
    }
  }
}
} // namespace

int day24_1(std::istream *input_file) {
  BlizzardMap map = BlizzardMap::from_stream(input_file);
  return simulate<TraversalDirection::BottomToTop>(map);
}

int day24_2(std::istream *input_file) {
  BlizzardMap map = BlizzardMap::from_stream(input_file);
  int trip_1 = simulate<TraversalDirection::BottomToTop>(map);
  int trip_2 = simulate<TraversalDirection::TopToBottom>(map, trip_1);
  return simulate<TraversalDirection::BottomToTop>(map, trip_2);
}

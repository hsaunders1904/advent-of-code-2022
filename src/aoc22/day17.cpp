#include "aoc22/utils.h"

#include <algorithm>
#include <array>
#include <bitset>
#include <iostream>
#include <istream>
#include <unordered_map>
#include <vector>

namespace {
using Rock = std::array<std::bitset<7>, 4>;

/* .......
   .......
   .......
   ..XXXX.  */
constexpr Rock SHAPE_MINUS = {0b0011110, 0, 0, 0};
/* .......
   ...X...
   ..XXX..
   ...X...  */
constexpr Rock SHAPE_PLUS = {0b0001000, 0b0011100, 0b0001000, 0};
/* .......
   ....X..
   ....X..
   ..XXX...  */
constexpr Rock SHAPE_J = {0b0011100, 0b0000100, 0b0000100, 0};
/* ..X....
   ..X....
   ..X....
   ..X....  */
constexpr Rock SHAPE_I = {0b0010000, 0b0010000, 0b0010000, 0b0010000};
/* .......
   .......
   ..XX...
   ..XX...  */
constexpr Rock SHAPE_O = {0b0011000, 0b0011000, 0, 0};
constexpr std::array<Rock, 5> SHAPES = {SHAPE_MINUS, SHAPE_PLUS, SHAPE_J, SHAPE_I,
                                        SHAPE_O};

struct Chamber {
  explicit Chamber(std::size_t rock);
  void new_rock(std::size_t rock_id);
  void apply_jet(char direction);
  bool move_down();
  void fix_rock();
  std::string to_string() const;
  void strip_top_of_pile();

  Rock rock;
  std::size_t rock_pos;
  std::vector<std::bitset<7>> pile{};

private:
  void move_left();
  bool can_move_left() const;
  void move_right();
  bool can_move_right() const;
  bool can_move_down() const;
  bool overlaps_pile(const Rock &rock) const;
};

Chamber::Chamber(const std::size_t rock) { new_rock(rock); }

void Chamber::new_rock(const std::size_t rock_id) {
  strip_top_of_pile();
  rock = SHAPES[rock_id % SHAPES.size()];
  rock_pos = pile.size() + 3;
  for (auto i = 0U; i < 3U + rock.size(); ++i) {
    pile.emplace_back(0);
  }
}

void Chamber::apply_jet(char direction) {
  switch (direction) {
  case '<':
    move_left();
    break;
  case '>':
    move_right();
    break;
  default:
    throw std::runtime_error("invalid jet: " + std::to_string(direction));
  }
}

bool Chamber::move_down() {
  if (rock_pos > 0 && can_move_down()) {
    --rock_pos;
    return true;
  }
  return false;
}

void Chamber::fix_rock() {
  for (auto i = 0U; i < rock.size(); ++i) {
    pile[rock_pos + i] |= rock[i];
  }
  strip_top_of_pile();
}

void Chamber::move_left() {
  if (can_move_left()) {
    for (auto i = 0U; i < rock.size(); ++i) {
      rock[i] = rock[i] << 1ULL;
    }
  }
}

void Chamber::move_right() {
  if (can_move_right()) {
    for (auto i = 0U; i < rock.size(); ++i) {
      rock[i] = rock[i] >> 1ULL;
    }
  }
}

bool Chamber::overlaps_pile(const Rock &r) const {
  auto pile_it = std::next(pile.begin(), rock_pos);
  for (auto i = 0U; i < r.size(); ++i) {
    if (((*pile_it) & r[i]).any()) {
      return true;
    }
    ++pile_it;
  }
  return false;
}

bool Chamber::can_move_left() const {
  // Check boundaries
  if (std::any_of(rock.begin(), rock.end(), [](auto row) { return row.test(6); })) {
    return false;
  }
  auto candidate_rock = rock;
  std::transform(candidate_rock.begin(), candidate_rock.end(), candidate_rock.begin(),
                 [](auto r) { return r << 1ULL; });
  return !overlaps_pile(candidate_rock);
}

bool Chamber::can_move_right() const {
  // Check boundaries
  if (std::any_of(rock.begin(), rock.end(), [](auto row) { return row.test(0); })) {
    return false;
  }
  auto candidate_rock = rock;
  std::transform(candidate_rock.begin(), candidate_rock.end(), candidate_rock.begin(),
                 [](auto r) { return r >> 1ULL; });
  return !overlaps_pile(candidate_rock);
}

bool Chamber::can_move_down() const {
  if (rock_pos == 0) {
    return false;
  }
  // Check pile collisions
  for (auto i = 0U; i < rock.size(); ++i) {
    if ((pile[rock_pos - 1 + i] & rock[i]).any()) {
      return false;
    }
  }
  return true;
}

void Chamber::strip_top_of_pile() {
  for (auto it = pile.rbegin(); it != pile.rend();) {
    if (!(it++)->any()) {
      pile.pop_back();
    } else {
      return;
    }
  }
}

std::string read_jets(std::istream *input_file) {
  std::string jets;
  std::getline(*input_file, jets);
  return jets;
}
} // namespace

int day17_1(std::istream *input_file) {
  auto jets = read_jets(input_file);
  constexpr std::size_t rocks_to_fall{2022};
  std::size_t jet_ctr{0};
  std::size_t rock_ctr{0};

  Chamber chamber(rock_ctr++);
  while (rock_ctr < rocks_to_fall + 1) {
    chamber.apply_jet(jets[jet_ctr++ % jets.size()]);
    if (!chamber.move_down()) {
      chamber.fix_rock();
      chamber.new_rock(rock_ctr++);
    }
  }
  chamber.strip_top_of_pile();
  return chamber.pile.size();
}

std::size_t day17_2(std::istream *input_file) {
  constexpr std::size_t rocks_to_fall{1'000'000'000'000};
  auto jets = read_jets(input_file);
  std::size_t jet_ctr{0};
  std::size_t rock_ctr{0};

  // Maps the 'state' of the chamber (jet no., rock no., & top 4 rows) to the rock no.
  // and the height at that state.
  std::unordered_map<std::vector<std::size_t>, std::pair<std::size_t, std::size_t>,
                     SequenceHasher<std::vector<std::size_t>>>
      seen_states;
  std::vector<std::size_t> heights;

  Chamber chamber(rock_ctr++);
  while (rock_ctr < rocks_to_fall + 1) {
    chamber.apply_jet(jets[jet_ctr++ % jets.size()]);
    if (!chamber.move_down()) {
      chamber.fix_rock();

      // Build the current state
      std::vector<std::size_t> state = {jet_ctr % jets.size(), rock_ctr % SHAPES.size()};
      for (auto i = 0U; i < 4; ++i) {
        // This only looks at the top 4 rows of the pile, so may not work for all inputs.
        state.emplace_back(chamber.pile[chamber.pile.size() - i - 1].to_ullong());
      }

      if (seen_states.find(state) == seen_states.end()) {
        seen_states.insert({state, {rock_ctr, chamber.pile.size()}});
      } else {
        // If we've seen the state before, we're in a cycle, so we can stop simulating
        auto [cycle_start, height_before_cycles] = seen_states[state];
        auto cycle_width = rock_ctr - cycle_start;
        auto n_before_cycle = cycle_start - 1;

        auto height_of_cycle = chamber.pile.size() - height_before_cycles;
        auto n_cycle_repeats = (rocks_to_fall - n_before_cycle) / cycle_width;
        std::size_t height_of_cycles = n_cycle_repeats * height_of_cycle;

        std::size_t remaining_cycles = (rocks_to_fall - n_before_cycle) % cycle_width;
        auto partial_cycle_height =
            heights[n_before_cycle + remaining_cycles - 1] - heights[n_before_cycle];

        return height_before_cycles + height_of_cycles + partial_cycle_height;
      }

      heights.emplace_back(chamber.pile.size());
      chamber.new_rock(rock_ctr++);
    }
  }
  return chamber.pile.size();
}

#include <iostream>
#include <istream>
#include <regex>
#include <sstream>

namespace {

std::string read_jets(std::istream *input_file) {
  std::string jets;
  std::getline(*input_file, jets);
  return jets;
}

using row_t = std::bitset<7>;

/*
.......
.......
.......
..XXXX.
*/
constexpr std::array<row_t, 4> SHAPE_MINUS = {0b0011110, 0, 0, 0};
/*
.......
...X...
..XXX..
...X...
*/
constexpr std::array<row_t, 4> SHAPE_PLUS = {0b0001000, 0b0011100, 0b0001000, 0};
/*
.......
....X..
....X..
..XXX...
*/
constexpr std::array<row_t, 4> SHAPE_J = {0b0011100, 0b0000100, 0b0000100, 0};
/*
..X....
..X....
..X....
..X....
*/
constexpr std::array<row_t, 4> SHAPE_I = {0b0010000, 0b0010000, 0b0010000, 0b0010000};
/*
.......
.......
..XX...
..XX...
*/
constexpr std::array<row_t, 4> SHAPE_O = {0b0011000, 0b0011000, 0, 0};

using Rock = std::array<row_t, 4>;

constexpr std::array<std::array<row_t, 4>, 5> SHAPES = {SHAPE_MINUS, SHAPE_PLUS, SHAPE_J,
                                                        SHAPE_I, SHAPE_O};

std::string regex_replace(const std::string &str, const std::string &pattern,
                          const std::string &new_str) {
  std::regex re(pattern);
  return std::regex_replace(str, re, new_str);
}

struct Chamber {
  Chamber();

  void new_rock();
  void apply_jet(char direction);
  bool move_down();
  void fix_rock();
  std::string to_string() const;

  Rock rock;
  std::size_t rock_pos;
  std::size_t num_turns{2022};
  std::size_t rock_ctr{0};
  std::vector<std::bitset<7>> pile{};
  void strip_top_of_pile();

private:
  void move_left();
  bool can_move_left() const;
  void move_right();
  bool can_move_right() const;
  bool can_move_down() const;
};

Chamber::Chamber() { new_rock(); }

void Chamber::new_rock() {
  strip_top_of_pile();
  rock = SHAPES[rock_ctr++ % SHAPES.size()];
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
    throw std::runtime_error("invalid jet");
  }
}

bool Chamber::move_down() {
  if (!can_move_down()) {
    return false;
  }
  if (rock_pos > 0) {
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
  if (!can_move_left()) {
    return;
  }
  for (auto i = 0U; i < rock.size(); ++i) {
    rock[i] = rock[i] << 1ULL;
  }
}

void Chamber::move_right() {
  if (!can_move_right()) {
    return;
  }
  for (auto i = 0U; i < rock.size(); ++i) {
    rock[i] = rock[i] >> 1ULL;
  }
}

bool Chamber::can_move_left() const {
  // Check boundaries
  if (std::any_of(rock.begin(), rock.end(), [](auto row) { return row.test(6); })) {
    return false;
  }
  // Check pile collisions
  auto pile_it = std::next(pile.begin(), rock_pos);
  for (auto i = 0U; i < rock.size(); ++i) {
    if (((*pile_it) & (rock[i] << 1UL)).any()) {
      return false;
    }
    ++pile_it;
  }
  return true;
}

bool Chamber::can_move_right() const {
  // Check boundaries
  if (std::any_of(rock.begin(), rock.end(), [](auto row) { return row.test(0); })) {
    return false;
  }
  // Check pile collisions
  auto pile_it = std::next(pile.begin(), rock_pos);
  for (auto i = 0U; i < rock.size(); ++i) {
    if ((*pile_it & (rock[i] >> 1ULL)).any()) {
      return false;
    }
    ++pile_it;
  }
  return true;
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

std::string Chamber::to_string() const {
  std::stringstream ss;
  auto chamber = pile;
  // Add the current rock to the pile
  for (auto i = 0U; i < rock.size(); ++i) {
    chamber[rock_pos + i] |= rock[i];
  }
  // Draw the pile
  for (auto it = chamber.rbegin(); it != chamber.rend(); ++it) {
    ss << "|" << *it << "|\n";
  }
  ss << "+-------+\n";

  std::string out = regex_replace(ss.str(), "0", ".");
  return regex_replace(out, "1", "#");
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
} // namespace

int day17_1(std::istream *input_file) {
  auto jets = read_jets(input_file);
  constexpr std::size_t rocks_to_fall{2022};
  std::size_t jet_ctr{0};

  Chamber chamber;
  while (chamber.rock_ctr < rocks_to_fall + 1) {
    chamber.apply_jet(jets[jet_ctr++ % jets.size()]);
    if (!chamber.move_down()) {
      chamber.fix_rock();
      chamber.new_rock();
    }
  }
  chamber.strip_top_of_pile();
  return chamber.pile.size();
}

int day17_2(std::istream *input_file) { return 1; }

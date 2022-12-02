#include "aoc22/aoc.h"

constexpr int OP_SCORE_CHAR_OFFSET = 23;
constexpr int SCORE_CHAR_OFFSET = 87;
constexpr int SCORE_WIN = 6;
constexpr int SCORE_DRAW = 3;

int get_score(const char player, const char opponent) {
  // Contribution to score from choice 1 - rock, 2 - paper, 3 - scissors
  int score = player - SCORE_CHAR_OFFSET;
  // Using the scores above to represent the choices, there are 6 cases when we
  // subtract the opponent's score from the players':
  // -2 (rock - scissors, player wins), -1 (rock - paper, paper - scissors,
  // loss) and so on. 0 is obviously a draw, and wins come with a difference of
  // -2 or 1.
  int diff = player - opponent - OP_SCORE_CHAR_OFFSET;
  if (diff == -2 || diff == 1) {
    score += SCORE_WIN;
  } else if (diff == 0) {
    score += SCORE_DRAW;
  }
  return score;
}

int day02_1(std::istream *input_file) {
  int score{0};
  for (std::string line; std::getline(*input_file, line);) {
    score += get_score(line[2], line[0]);
  }
  return score;
}

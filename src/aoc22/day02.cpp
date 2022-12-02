#include "aoc22/aoc.h"

/**
 * @brief Get the score for the given rock-paper-scissor choices.
 *  0=rock, 1=paper, 2=scissors
 *
 *  Score 0 for a loss, 3 for a draw, 6 for a win.
 *  In addition, score 1 for rock, 2 for paper, 3 for scissors.
 */
int get_score(const int player, const int opponent) {
  // result: loss=2, win=1, loss=0
  int result = (player - opponent + 3) % 3;
  return 1 + player + ((result + 1) % 3) * 3;
}

int day02_1(std::istream *input_file) {
  int score{0};
  for (std::string line; std::getline(*input_file, line);) {
    score += get_score(line[2] - 'X', line[0] - 'A');
  }
  return score;
}

int day02_2(std::istream *input_file) {
  int score{0};
  for (std::string line; std::getline(*input_file, line);) {
    int op = line[0] - 'A';
    score += get_score((op + (line[2] - 'X' + 2) % 3) % 3, op);
  }
  return score;
}

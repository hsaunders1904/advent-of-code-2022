#include <array>
#include <istream>
#include <string>

namespace {
int priority(const char character) {
  if (character >= 'a' && character <= 'z') {
    return character - 'a' + 1;
  } else {
    return character - 'A' + 27;
  }
}

std::array<std::string, 3> read_3_lines(std::istream *stream) {
  std::array<std::string, 3> lines;
  for (auto i = 0U; i < 3; ++i) {
    std::getline(*stream, lines[i]);
  }
  return lines;
}
} // namespace

int day03_1(std::istream *input_file) {
  int priority_sum{0};
  for (std::string line; std::getline(*input_file, line);) {
    for (auto i = 0U; i < line.size() / 2; ++i) {
      if (line.find_first_of(line[i], line.size() / 2) != std::string::npos) {
        priority_sum += priority(line[i]);
        break;
      }
    }
  }
  return priority_sum;
}

int day03_2(std::istream *input_file) {
  int priority_sum{0};
  while (!input_file->eof()) {
    auto lines = read_3_lines(input_file);
    for (auto i = 0U; i < lines[0].size(); ++i) {
      if (lines[1].find_first_of(lines[0][i]) != std::string::npos) {
        if (lines[2].find_first_of(lines[0][i]) != std::string::npos) {
          priority_sum += priority(lines[0][i]);
          break;
        }
      }
    }
  }
  return priority_sum;
}

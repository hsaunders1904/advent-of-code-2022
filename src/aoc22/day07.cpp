#include "aoc22/aoc.h"

#include <limits>
#include <map>
#include <numeric>

using str_vec_iterator = std::vector<std::string>::iterator;

namespace {
inline bool is_command(const std::vector<std::string> &line) { return line[0] == "$"; }

inline std::vector<std::string> read_line(std::istream *stream) {
  std::string line;
  std::getline(*stream, line);
  return split(line, ' ');
}

std::string to_string(str_vec_iterator begin, str_vec_iterator end) {
  std::string out = "/";
  for (; begin != end; ++begin) {
    out += *begin + "/";
  }
  return out;
}

std::map<std::string, int> directory_sizes(std::istream *input_file) {
  std::map<std::string, int> dir_sizes;
  std::vector<std::string> current_dir;
  auto line = read_line(input_file);
  while (!input_file->eof()) {
    if (is_command(line)) {
      if (line[1] == "cd") {
        if (line[2] == "/") {
          current_dir = {};
        } else if (line[2] == "..") {
          current_dir.pop_back();
        } else {
          current_dir.push_back(line[2]);
        }
        line = read_line(input_file);
      } else if (line[1] == "ls") {
        line = read_line(input_file);
        while (!(line.empty() || is_command(line))) {
          if (line[0][0] != 'd') {
            const auto file_size = std::stoi(line[0]);
            for (auto i = 0U; i < current_dir.size() + 1; ++i) {
              auto dir_name =
                  to_string(current_dir.begin(), std::next(current_dir.begin(), i));
              dir_sizes[dir_name] += file_size;
            }
          }
          line = read_line(input_file);
        }
      }
    }
  }
  return dir_sizes;
}
} // namespace

int day07_1(std::istream *input_file) {
  const auto dir_sizes = directory_sizes(input_file);
  return std::accumulate(dir_sizes.begin(), dir_sizes.end(), 0,
                         [](const auto sum, const auto &pair) {
                           return pair.second <= 1e5 ? sum + pair.second : sum;
                         });
}

int day07_2(std::istream *input_file) {
  constexpr int disk_space{70000000};
  constexpr int space_needed{30000000};
  const auto dir_sizes = directory_sizes(input_file);
  const auto space_to_free = space_needed - (disk_space - dir_sizes.at("/"));

  auto dir_size_to_delete = std::numeric_limits<int>().max();
  for (const auto &[dir_name, dir_size] : dir_sizes) {
    if (dir_size < dir_size_to_delete && dir_size >= space_to_free) {
      dir_size_to_delete = dir_size;
    }
  }
  return dir_size_to_delete;
}

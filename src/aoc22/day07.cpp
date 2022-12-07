#include "aoc22/aoc.h"

#include <limits>
#include <map>
#include <numeric>

namespace {
inline bool is_command(const std::vector<std::string> &line) {
  return line[0] == "$";
}

inline std::vector<std::string> read_line(std::istream *stream) {
  std::string line;
  std::getline(*stream, line);
  if (line.empty()) {
    return {};
  }
  return split(line, ' ');
}

std::string strip_dir(const std::string &path) {
  std::string new_path = path.substr(0, path.find_last_of('/'));
  if (path[path.size() - 1] == '/') {
    new_path = new_path.substr(0, new_path.find_last_of('/'));
  }
  return new_path;
}

std::map<std::string, int> directory_sizes(std::istream *input_file) {
  std::map<std::string, int> dir_sizes;
  std::string current_dir;
  auto line = read_line(input_file);
  while (true) {
    if (line.empty()) {
      break;
    }
    if (is_command(line)) {
      if (line[1] == "cd") {
        if (line[2] == "/") {
          current_dir = "/";
        } else if (line[2] == "..") {
          current_dir = strip_dir(current_dir) + "/";
        } else {
          current_dir += line[2] + "/";
        }
        line = read_line(input_file);
      } else if (line[1] == "ls") {
        while (true) {
          line = read_line(input_file);
          if (line.empty() || is_command(line)) {
            break;
          }
          if (line[0][0] != 'd') {
            const auto file_size = std::stoi(line[0]);
            auto parts = split(current_dir, '/');
            std::string dir_name = "";
            for (auto i = 0U; i < parts.size(); ++i) {
              dir_name += parts[i] + "/";
              dir_sizes[dir_name] += file_size;
            }
          }
        }
      }
    }
  }
  return dir_sizes;
}
} // namespace

int day07_1(std::istream *input_file) {
  auto dir_sizes = directory_sizes(input_file);
  int sums{0};
  for (const auto &p : dir_sizes) {
    if (p.second <= 100000) {
      sums += p.second;
    }
  }
  return sums;
}

int day07_2(std::istream *input_file) {
  constexpr int disk_space{70000000};
  constexpr int space_needed{30000000};
  auto dir_sizes = directory_sizes(input_file);
  auto space_to_free = space_needed - (disk_space - dir_sizes.at("/"));

  auto dir_size_to_delete = std::numeric_limits<int>().max();
  for (const auto &[dir_name, dir_size] : dir_sizes) {
    if (dir_size < dir_size_to_delete && dir_size >= space_to_free) {
      dir_size_to_delete = dir_size;
    }
  }
  return dir_size_to_delete;
}

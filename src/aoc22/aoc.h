#pragma once

#include <fstream>

using day_function = std::function<int(std::istream *)>;

std::ifstream open_file(const std::string &path);
int run_day(int argc, char *argv[], day_function, day_function);

int day01_1(std::istream *input_file);
int day01_2(std::istream *input_file);
int day02_1(std::istream *input_file);
int day02_2(std::istream *input_file);
int day03_1(std::istream *input_file);
int day03_2(std::istream *input_file);

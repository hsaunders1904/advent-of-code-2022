#pragma once

#include <fstream>

std::ifstream open_file(const std::string &path);
std::string parse_one_cli_arg(int argc, char *argv[]);

int day01_1(std::istream *input_file);
int day01_2(std::istream *input_file);
int day02_1(std::istream *input_file);
int day02_2(std::istream *input_file);
int day03_1(std::istream *input_file);

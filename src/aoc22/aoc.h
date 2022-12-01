#pragma once

#include <fstream>
#include <istream>

std::ifstream open_file(const std::string &path);
std::string parse_one_cli_arg(int argc, char *argv[]);

int day01(std::istream *input_file);

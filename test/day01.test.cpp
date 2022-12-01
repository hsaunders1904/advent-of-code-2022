#include "aoc22/aoc.h"

#include <gtest/gtest.h>

#include <sstream>

TEST(day01, CorrectResult) {
  std::stringstream stream;
  stream << "1000\n\n2000\n3000\n\n4000\n\n5000\n6000\n\n7000\n8000\n9000\n\n10"
            "000\n\n";

  EXPECT_EQ(day01(&stream), 24000);
}

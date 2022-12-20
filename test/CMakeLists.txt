find_package(GTest)
if (NOT ${GTest_FOUND})
  return()
endif()

add_executable(aoc-test "aoc22.test.cpp" "test_data.h")
target_link_libraries(aoc-test
  PRIVATE GTest::gtest_main
  PRIVATE aoc
)

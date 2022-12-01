find_package(GTest)
if (NOT ${GTest_FOUND})
  return()
endif()

set(SRC
  "day01.test.cpp"
)

add_executable(aoc-test ${SRC})
target_link_libraries(aoc-test
  PRIVATE GTest::gtest_main
  PRIVATE aoc
)

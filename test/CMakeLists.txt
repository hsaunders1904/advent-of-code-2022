find_package(GTest)
if(GTest_FOUND)
  add_executable(aoc-test "aoc22.test.cpp" "test_data.h")
  target_link_libraries(aoc-test
  PRIVATE GTest::gtest_main
  PRIVATE aoc
  )
endif()

find_package(benchmark)
if(benchmark_FOUND)
  add_executable(aoc-benchmark "benchmark.cpp")
  target_link_libraries(aoc-benchmark
  PRIVATE benchmark::benchmark
  PRIVATE aoc
  )
endif()

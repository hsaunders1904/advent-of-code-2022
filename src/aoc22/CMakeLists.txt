set(SRC
  "day01.cpp"
  "common.cpp"
)
add_library(aoc SHARED ${SRC} "aoc.h")

set(EXES
  "day01"
)
foreach(exe ${EXES})
  add_executable(${exe} "${exe}.main.cpp")
  target_link_libraries(${exe} PRIVATE aoc)
endforeach()


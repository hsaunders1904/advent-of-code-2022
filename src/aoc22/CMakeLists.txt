set(DAYS
  01
  02
  03
  04
  05
  06
  07
  08
  09
  10
  11
  12
  13
  14
  15
  16
  17
  18
  19
  20
  21
)
foreach(day ${DAYS})
  list(APPEND SRC "day${day}.cpp")
  add_executable("day${day}" "day${day}.main.cpp")
  target_link_libraries("day${day}" PRIVATE aoc)
endforeach()

add_library(aoc SHARED ${SRC} "aoc.h" "utils.cpp" "utils.h")
set_target_properties(aoc PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)

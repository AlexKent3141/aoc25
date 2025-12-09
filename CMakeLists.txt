cmake_minimum_required(VERSION 3.16)

project(aoc25)

function(create_day_target name)
  add_executable(${name} "main.c")
  set_target_properties(${name}
    PROPERTIES
      LANGUAGE C
      C_STANDARD 90)

  target_compile_options(${name}
    PRIVATE
      -Wall -Wextra -Wpedantic -Werror -O3)
endfunction()

add_subdirectory(common)

# Add directories for each day here.
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)
add_subdirectory(day5)
add_subdirectory(day6)
add_subdirectory(day7)
add_subdirectory(day8)
add_subdirectory(day9)

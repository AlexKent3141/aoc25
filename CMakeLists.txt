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
      -Wall -Wextra -Wpedantic -Werror)
endfunction()

add_subdirectory(common)

# Add directories for each day here.
add_subdirectory(day1)
add_subdirectory(day2)
add_subdirectory(day3)
add_subdirectory(day4)

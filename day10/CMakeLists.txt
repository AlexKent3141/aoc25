create_day_target(day10)

# Temporary separate target for Part 2 in C++.
add_executable(day10_p2 p2.cpp)

target_compile_options(day10_p2 PRIVATE -O3 -std=c++20)

#---Adding examples subdirectories explicitly
#   and a custom target to for building all basic examples -------------
cmake_minimum_required(VERSION 3.16...3.27)

add_subdirectory(B1)
add_subdirectory(B2)
add_subdirectory(B3)
add_subdirectory(B4)
add_subdirectory(B5)

add_custom_target(basic DEPENDS exampleB1 B2 B3 B4 exampleB5)

cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Shading")

# Add externals
add_subdirectory("external/lvk")
add_subdirectory("external/assimp")

# Add Shading Modules
add_subdirectory("phong")
add_subdirectory("gouraud")
add_subdirectory("flat")


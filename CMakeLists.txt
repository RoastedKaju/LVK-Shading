cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Shading")

# Add externals
add_subdirectory("external/lvk")
add_subdirectory("external/assimp")

# Shader files
file(GLOB_RECURSE SHADER_FILES "${CMAKE_SOURCE_DIR}/resources/shaders/*")

# Add Shading Modules
add_subdirectory("phong")
add_subdirectory("gooch")


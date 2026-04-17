cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Shading")

# Turn off LVK Samples
set(LVK_WITH_SAMPLES OFF CACHE BOOL "" FORCE)
set(LVK_WITH_SAMPLES_ANDROID OFF CACHE BOOL "" FORCE)
set(LVK_DEPLOY_SCREENSHOT_TESTS OFF CACHE BOOL "" FORCE)

# Add externals
add_subdirectory("external/lvk")
add_subdirectory("external/assimp")

# Add Shading Modules
add_subdirectory("phong")
add_subdirectory("gouraud")
add_subdirectory("flat")
add_subdirectory("toon")
add_subdirectory("psx")
add_subdirectory("skybox")
add_subdirectory("post_process")


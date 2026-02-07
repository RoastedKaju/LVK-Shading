cmake_minimum_required(VERSION 3.14)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project("Shading")

# Source files
file(GLOB_RECURSE SRC_FILES "${CMAKE_SOURCE_DIR}/src/*.h" "${CMAKE_SOURCE_DIR}/src/*.cpp")

# Shader files
file(GLOB_RECURSE SHADER_FILES "${CMAKE_SOURCE_DIR}/resources/shaders/*")

add_executable(${PROJECT_NAME} ${SRC_FILES} ${SHADER_FILES})

# Group shader files
source_group("Shaders" FILES ${SHADER_FILES})

# Compile definations
target_compile_definitions(${PROJECT_NAME} PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources")
target_compile_definitions(${PROJECT_NAME} PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/resources/shaders")


add_subdirectory("external/lvk")
add_subdirectory("external/assimp")

target_link_libraries(${PROJECT_NAME} PRIVATE glfw LVKLibrary LVKstb ktx assimp)

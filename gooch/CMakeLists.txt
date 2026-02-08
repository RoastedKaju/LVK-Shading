set(MODULE_NAME "Gooch")

# Source files
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Group shader files
source_group("Shaders" FILES ${SHADER_FILES})

add_executable(${MODULE_NAME} ${SRC_FILES} ${SHADER_FILES})

# Link Libraries
target_link_libraries(${MODULE_NAME} PRIVATE glfw LVKLibrary LVKstb ktx assimp)

# Compile definations
target_compile_definitions(${MODULE_NAME} PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources")
target_compile_definitions(${MODULE_NAME} PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/resources/shaders")

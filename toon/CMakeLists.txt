set(MODULE_NAME "Toon")
set(SHADER_DIR "${CMAKE_SOURCE_DIR}/resources/shaders")

# Source files
file(GLOB_RECURSE SRC_FILES "${CMAKE_CURRENT_SOURCE_DIR}/src/*.h" "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")

# Shader files
set(SHADER_FILES "${SHADER_DIR}/toon.frag" "${SHADER_DIR}/toon.vert" "${SHADER_DIR}/outline.vert" "${SHADER_DIR}/outline.frag" "${SHADER_DIR}/common.sp")
source_group("Shaders" FILES "${SHADER_DIR}/toon.frag" "${SHADER_DIR}/toon.vert" "${SHADER_DIR}/outline.vert" "${SHADER_DIR}/outline.frag" "${SHADER_DIR}/common.sp")
# Shared files
file(GLOB_RECURSE SHARED_FILES "${CMAKE_SOURCE_DIR}/shared/*.h")
source_group("Shared" FILES ${SHARED_FILES})

add_executable(${MODULE_NAME} ${SRC_FILES} ${SHADER_FILES} ${SHARED_FILES})

# Include shared directory
target_include_directories(${MODULE_NAME} PRIVATE "${CMAKE_SOURCE_DIR}/shared")

# Link Libraries
target_link_libraries(${MODULE_NAME} PRIVATE glfw LVKLibrary LVKstb ktx assimp)

# Compile definations
target_compile_definitions(${MODULE_NAME} PRIVATE RESOURCE_DIR="${CMAKE_SOURCE_DIR}/resources")
target_compile_definitions(${MODULE_NAME} PRIVATE SHADER_DIR="${CMAKE_SOURCE_DIR}/resources/shaders")

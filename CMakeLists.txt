cmake_minimum_required(VERSION 3.19)
project(forge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
### Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw" "glfw")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/yaml-cpp" "yaml-cpp")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/assimp" "assimp")

file(GLOB_RECURSE src_files "src/*.h" "src/*.cpp")
file(GLOB imgui_files "external/imgui/*.h" "external/imgui/*.cpp")

add_executable(forge
        external/stb/stb_image.h
        external/entt/single_include/entt/entt.hpp
        ${imgui_files}
        ${src_files})

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glm" "glm")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

target_link_libraries(forge glfw ${GLFW_LIBRARIES} yaml-cpp glm assimp)


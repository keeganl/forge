cmake_minimum_required(VERSION 3.19)
project(forge)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_VERBOSE_MAKEFILE ON)
### Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glfw" "glfw")


add_executable(forge
        src/main.cpp
        external/imgui/imconfig.h
        external/imgui/imgui.h
        external/imgui/imgui.cpp
        external/imgui/imgui_demo.cpp
        external/imgui/imgui_draw.cpp
        external/imgui/imgui_internal.h
        external/imgui/imgui_tables.cpp
        external/imgui/imgui_widgets.cpp
        external/imgui/imstb_truetype.h
        external/imgui/imstb_textedit.h
        external/imgui/imstb_rectpack.h
        external/imgui/imgui_impl_glfw.h
        external/imgui/imgui_impl_glfw.cpp
        external/imgui/imgui_impl_opengl3.h
        external/imgui/imgui_impl_opengl3.cpp
        external/imgui/imgui_impl_win32.h
        external/imgui/imgui_impl_win32.cpp
        )

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

target_link_libraries(forge glfw ${GLFW_LIBRARIES})


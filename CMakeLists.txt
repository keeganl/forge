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
        external/imgui/ImCurveEdit.h
        external/imgui/ImCurveEdit.cpp
        external/imgui/ImGradient.h
        external/imgui/ImGradient.cpp
        external/imgui/ImGuizmo.h
        external/imgui/ImGuizmo.cpp
        external/imgui/imfilebrowser.h
        external/stb/stb_image.h
        external/entt/single_include/entt/entt.hpp
        src/editor/utils/imgui-layer/GuiLayer.cpp
        src/editor/utils/imgui-layer/GuiLayer.h
        src/editor/utils/shader-manager/Shader.cpp
        src/editor/utils/shader-manager/Shader.h
        src/editor/camera/Camera.h
        src/editor/camera/Camera.cpp
        src/editor/mesh/helpers/Vertex.h
        src/editor/mesh/helpers/Texture.h
        src/editor/mesh/Mesh.h
        src/editor/renderer/Renderer.cpp
        src/editor/renderer/Renderer.h
        src/editor/mesh/Model.h external/imgui/imfilebrowser.h
        src/editor/light/Light.cpp
        src/editor/light/Light.h
        src/editor/utils/Serializer.cpp
        src/editor/utils/Serializer.h src/editor/Editor.cpp src/editor/Editor.h)

set(GLAD_DIR "${CMAKE_CURRENT_SOURCE_DIR}/external/glad")
add_library("glad" "${GLAD_DIR}/src/glad.c")
target_include_directories("glad" PRIVATE "${GLAD_DIR}/include")
target_include_directories(${PROJECT_NAME} PRIVATE "${GLAD_DIR}/include")
add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/external/glm" "glm")
target_link_libraries(${PROJECT_NAME} "glad" "${CMAKE_DL_LIBS}")

target_link_libraries(forge glfw ${GLFW_LIBRARIES} yaml-cpp glm assimp)


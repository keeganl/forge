//
// Created by keega on 6/5/2021.
//

#ifndef FORGE_IMGUILAYER_H
#define FORGE_IMGUILAYER_H
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_glfw.h"
#include "../../../external/imgui/imgui_impl_opengl3.h"
#include "../../../external/imgui/imgui_internal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


class ImGuiLayer {
public:
    void createContext(GLFWwindow *window);
    void createDockspace();
    void createPerformanceWindow();
    void createGeometryEditorWindow();

};


#endif //FORGE_IMGUILAYER_H

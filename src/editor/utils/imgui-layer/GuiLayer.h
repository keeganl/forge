#ifndef FORGE_GUILAYER_H
#define FORGE_GUILAYER_H
#include "../../../../external/imgui/imgui.h"
#include "../../../../external/imgui/imgui_impl_glfw.h"
#include "../../../../external/imgui/imgui_impl_opengl3.h"
#include "../../../../external/imgui/imgui_internal.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace GuiLayer {
    void createContext(GLFWwindow *window);
    void startFrame();
    void createDockspace();
    void createPerformanceWindow();
    void createGeometryEditorWindow();

};


#endif //FORGE_GUILAYER_H

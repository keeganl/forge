#ifndef FORGE_GUILAYER_H
#define FORGE_GUILAYER_H
#include "../../../external/imgui/imgui.h"
#include "../../../external/imgui/imgui_impl_glfw.h"
#include "../../../external/imgui/imgui_impl_opengl3.h"
#include "../../../external/imgui/imgui_internal.h"
#include "../entity/camera/EditorCamera.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


namespace GuiLayer {
    void createContext(GLFWwindow *window);
    void startFrame();
    void createDockspace();
    void createPerformanceWindow();
    void showSettings(bool* p_open);
    void drawDebugEvents();
    void drawMenubar(bool &filePopup, bool &savePopup, bool &settingsPopup);
    void createGeometryEditorWindow();
    void drawCameraEditor(EditorCamera &camera);

    // helpers
    void HelpMarker(const char* desc);
};


#endif //FORGE_GUILAYER_H

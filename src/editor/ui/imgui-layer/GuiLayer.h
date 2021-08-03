#ifndef FORGE_GUILAYER_H
#define FORGE_GUILAYER_H
#include "../../../../external/imgui/imgui.h"
#include "../../../../external/imgui/imgui_impl_glfw.h"
#include "../../../../external/imgui/imgui_impl_opengl3.h"
#include "../../../../external/imgui/imgui_internal.h"
#include "../../mesh/Model.h"
#include "../../light/Light.h"
#include "../../camera/Camera.h"
#include "../../settings/Settings.h"
#include "../ModalManager/ModalManager.h"
#include "../../utils/Serializer.h"



#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>


namespace GuiLayer {
    void createContext(GLFWwindow *window);
    void startFrame();
    void createDockspace();
    void createPerformanceWindow();
    void drawModelPropertiesPanel(std::vector<std::shared_ptr<Model>> &scenes);
    void drawCameraPropertiesPanel(Camera &camera);
    void drawDebugEventsPanel();
    void drawScenePanel(unsigned int &textureColorbuffer, bool &firstMouse, float &deltaTime, Camera &camera, Keymap &keymap, std::vector<std::shared_ptr<Model>> &scenes);
    void drawMenubar(Settings &settings, ModalManager &modalManager, std::vector<std::shared_ptr<Model>> &scenes, Camera &camera);
};


#endif //FORGE_GUILAYER_H

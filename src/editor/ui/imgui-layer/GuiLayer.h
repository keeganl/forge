#ifndef FORGE_GUILAYER_H
#define FORGE_GUILAYER_H
#include "../../../../external/imgui/imgui.h"
#include "../../../../external/imgui/imgui_impl_glfw.h"
#include "../../../../external/imgui/imgui_impl_opengl3.h"
#include "../../../../external/imgui/imgui_internal.h"
#include "../../scene/Scene.h"
#include "../../camera/Camera.h"
#include "../../settings/Settings.h"
#include "../ModalManager/ModalManager.h"
#include "../../utils/Serializer.h"
#include "../textures/UITexture.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <memory>


namespace GuiLayer {
    void createContext(GLFWwindow *window);
    void cleanup();
    void startFrame();
    void createDockspace();
    void createPerformanceWindow();
    void drawModelPropertiesPanel(Scene &scene, std::map<std::string, UITexture> &uiTextures, ModalManager &modalManager);
    void drawCameraPropertiesPanel(Camera &camera);
    void drawDebugEventsPanel();
    void drawScenePanel(unsigned int &textureColorbuffer, bool &firstMouse, float &deltaTime, Scene &scene, Keymap &keymap);
    void drawMenubar(Settings &settings, ModalManager &modalManager, Scene &scene);
    void drawAssetBrowser(Settings &settings, std::map<std::string, UITexture> &uiTextures);
};


#endif //FORGE_GUILAYER_H
